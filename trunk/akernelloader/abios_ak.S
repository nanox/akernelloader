/*  Akernelloader TEAM
    akaloaderadmin@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. 
 */




#ifndef ASM_FILE
#define ASM_FILE

#include "prototypes/os_ak/linux_ak.h"

.text

.global  getkey_ak, cls_ak, putchar_ak, A20_line;
.global     bios_rw_lba_ak, bios_rw_chs_ak, get_drive_param_ak ;
.global  bios_memory_ak, bios_bigmem_ak ;
.global  bios_memory_ak, bios_bigmem_ak, boot_kernel_ak ;
.global linux_base_ptr, boot_linux_ak  ;

/*If an error occurs, return
  non-zero, otherwise zero. */
linux_base_ptr : .long 0 

bios_rw_chs_ak:
    pushl    %ebp
    movl    %esp, %ebp

    pushl    %ebx
    pushl    %edi
    pushl    %esi

    /* set up CHS information */
    movl    0x10(%ebp), %eax
    movb    %al, %ch
    movb    0x18(%ebp), %al
    shlb    $2, %al
    shrw    $2, %ax
    movb    %al, %cl
    movb    0x14(%ebp), %dh
    /* drive */
    movb    0xc(%ebp), %dl
    /* segment */
    movw    0x20(%ebp), %bx
    /* save nsec and ah to %di */
    movb    0x8(%ebp), %ah
        add     $0x02,%ah        /* 0x02 is read disk and 0x03 write */

    movb    0x1c(%ebp), %al
    movw    %ax, %di
    /* enter real mode */
    call    leave_protected_mode

    .code16
    movw    %bx, %es
    xorw    %bx, %bx
    movw    $3, %si        /* attempt at least three times */

1:    
    movw    %di, %ax
    int    $0x13        /* do the operation */
    jnc    2f        /* check if successful */

    movb    %ah, %bl    /* save return value */
    /* if fail, reset the disk system */
    xorw    %ax, %ax
    int    $0x13
    
    decw    %si
    cmpw    $0, %si
    je    2f
    xorb    %bl, %bl
    jmp    1b        /* retry */
2:    
    /* back to protected mode */
    DATA32    call    enter_protected_mode
    .code32

    movb    %bl, %al    /* return value in %eax */
    
    popl    %esi
    popl    %edi
    popl    %ebx
    popl    %ebp

    ret

 /* If an error occurs, return
    non-zero, otherwise zero. */

bios_rw_lba_ak:
    pushl    %ebp
    movl    %esp, %ebp

    pushl    %esi
    pushl    %ebx

    /* compute the address of disk_address_packet */
    movl    0x10(%ebp), %eax
    movw    %ax, %si
    xorw    %ax, %ax
    shrl    $4, %eax
    movw    %ax, %cx    /* save the segment to cx */

    /* drive */
    movb    0xc(%ebp), %dl
    /* ax */
    movw    0x8(%ebp), %bx
    /* enter real mode */
    call    leave_protected_mode
    
    .code16
        add    $0x42,%bx
    movw    %bx, %ax
        shlw    $0x08,%ax
    movw    %cx, %ds
    int    $0x13        /* do the operation */
    movb    %ah, %dl    /* save return value */
    /* clear the data segment */
    xorw    %ax, %ax
    movw    %ax, %ds
    /* back to protected mode */
    DATA32    call  enter_protected_mode
    .code32

    movb    %dl, %al    /* return value in %eax */

    popl    %ebx
    popl    %esi
    popl    %ebp

    ret


putchar_ak:
    movl    0x4(%esp), %edx
    pusha

    movb    $0x07, %bl

    
    call    leave_protected_mode
    .code16
    movb    %dl, %al
    xorb    %bh, %bh


    /* use teletype output if control character */
    cmpb    $0x7, %al
    je    1f
    cmpb    $0x8, %al
    je    1f
    cmpb    $0xa, %al
    je    new_line
    cmpb    $0xd, %al
    je    1f

    /* save the character and the attribute on the stack */
    pushw    %ax
    pushw    %bx
    
    /* get the current position */
    movb    $0x3, %ah
    int    $0x10

    /* check the column with the width */
    cmpb    $79, %dl
    jl    2f
    
    /* print CR and LF, if next write will exceed the width */    
    movw    $0x0e0d, %ax
    int    $0x10
    movb    $0x0a, %al
    int    $0x10
    
    /* get the current position */
    movb    $0x3, %ah
    int    $0x10

2:    
    /* restore the character and the attribute */
    popw    %bx
    popw    %ax
    
    /* write the character with the attribute */
    movb    $0x9, %ah
    movw    $1, %cx
    int    $0x10

    /* move the cursor forward */
    incb    %dl
    movb    $0x2, %ah
    int    $0x10

    jmp    3f

    
1:    movb    $0xe, %ah
    int    $0x10
        jmp     3f

new_line:
        
        movb    $0x0d,%al
        movb    $0xe, %ah
    int    $0x10
        movb    $0x0a, %al
    int    $0x10
    
3:    DATA32    call  enter_protected_mode
    .code32
    
    popa
    ret

cls_ak:
    push    %ebp
    push    %ebx                    /* save EBX */

    call    leave_protected_mode
    .code16

    /* move the cursor to the beginning */
    movb    $0x02, %ah
    xorb    %bh, %bh
    xorw    %dx, %dx
    int    $0x10

    /* write spaces to the entire screen */
    movw    $0x0920, %ax
    movw    $0x07, %bx
    movw    $(80 * 25), %cx
        int    $0x10

    /* move back the cursor */
    movb    $0x02, %ah
    int    $0x10

    call    enter_protected_mode
    .code32

    pop    %ebx
    pop    %ebp
    ret

getkey_ak:
    
         .code32      
        
    call    leave_protected_mode
    .code16
        movb $0x00,%al        # Set zero
    movb $0x00,%ah    
    int  $0x16
        
    
     call    enter_protected_mode
    .code32

    movw    %dx, %ax
    ret

/*  Read drive parameters
    Return the geometry of DRIVE in CYLINDERS, HEADS and SECTORS. If an
    error occurs, then return non-zero, otherwise zero.
*/

get_drive_param_ak:

    pushl    %ebp
    movl    %esp, %ebp

    pushl    %ebx
    pushl    %edi

    /* drive */
    movb    0x8(%ebp), %dl
    /* enter real mode */
    call    leave_protected_mode

    .code16
    movb    $0x8, %ah
    int    $0x13        /* do the operation */
    /* check if successful */
    testb    %ah, %ah
    jnz    1f
    /* bogus BIOSes may not return an error number */
    testb    $0x3f, %cl    /* 0 sectors means no disk */
    jnz    1f        /* if non-zero, then succeed */
    /* XXX 0x60 is one of the unused error numbers */
    movb    $0x60, %ah
1:
    movb    %ah, %bl    /* save return value in %bl */
    /* back to protected mode */
    DATA32    call    enter_protected_mode
    .code32

    /* restore %ebp */
    leal    0x8(%esp), %ebp
    
    /* heads */
    movb    %dh, %al
    incl    %eax        /* the number of heads is counted from zero */
    movl    0x10(%ebp), %edi
    movl    %eax, (%edi)

    /* sectors */
    xorl    %eax, %eax
    movb    %cl, %al
    andb    $0x3f, %al
    movl    0x14(%ebp), %edi
    movl    %eax, (%edi)

    /* cylinders */
    shrb    $6, %cl
    movb    %cl, %ah
    movb    %ch, %al
    incl    %eax        /* the number of cylinders is 
                   counted from zero */
    movl    0xc(%ebp), %edi
    movl    %eax, (%edi)

    xorl    %eax, %eax
    movb    %bl, %al    /* return value in %eax */

    popl    %edi
    popl    %ebx
    popl    %ebp

    ret



#define K_RDWR      0x60    /* keyboard data & cmds (read/write) */
#define K_STATUS    0x64    /* keyboard status */
#define K_CMD        0x64    /* keybd ctlr command (write-only) */

#define K_OBUF_FUL     0x01    /* output buffer full */
#define K_IBUF_FUL     0x02    /* input buffer full */

#define KC_CMD_WIN    0xd0    /* read  output port */
#define KC_CMD_WOUT    0xd1    /* write output port */
#define KB_OUTPUT_MASK  0xdd    /* enable output buffer full interrupt
                   enable data line
                   enable clock line */
#define KB_A20_ENABLE   0x02

A20_line:
    /* first, try a BIOS call */
    pushl    %ebp
    movl    8(%esp), %edx
    
    call    leave_protected_mode
    
    .code16
    movw    $0x2400, %ax
    testw    %dx, %dx
    jz    1f
    incw    %ax
1:    stc
    int    $0x15
    jnc    2f

    /* set non-zero if failed */
    movb    $1, %ah

    /* save the status */
2:    movb    %ah, %dl

    DATA32    call    enter_protected_mode
    .code32

    popl    %ebp
    testb    %dl, %dl
    jnz    3f
    ret

3:    /* use keyboard controller */
    pushl    %eax

	call    loop1

    movb    $KC_CMD_WOUT, %al
    outb    $K_CMD

loopint1:
	inb	$K_STATUS
	andb	$K_IBUF_FUL, %al
	jnz	loopint1

	movb	$KB_OUTPUT_MASK, %al
	cmpb	$0, 0x8(%esp)
	jz	doit

	orb	$KB_A20_ENABLE, %al
doit:
	outb	$K_RDWR

	call	loop1

	/* output a dummy command (USB keyboard hack) */
	movb	$0xff, %al
	outb	$K_CMD
	call	loop1
	
	popl	%eax
	ret

loop1:
	inb	$K_STATUS
	andb	$K_IBUF_FUL, %al
	jnz	loop1

loop2:
	inb	$K_STATUS
	andb	$K_OBUF_FUL, %al
	jz	loop2ret
	inb	$K_RDWR
	jmp	loop2

loop2ret:
	ret




bios_memory_ak:
	push	%ebp
	push	%ebx

	mov	0xc(%esp), %ebx

	call	leave_protected_mode	// enter real mode 
	.code16

	cmpb	$0x1, %bl
	DATA32	je	m_ext

	int	$0x12       // System Memory Size KB
	DATA32	jmp	m_done

m_ext:
	movb	$0x88, %ah  // Get memory size (extended mem, kB)
	int	$0x15

m_done:
	movw	%ax, %bx

	DATA32	call enter_protected_mode
	.code32

	movw	%bx, %ax
	pop	%ebx
	pop	%ebp
	ret


// Big Memory size (over 64 kB)

bios_bigmem_ak :
	push	%ebp
	push	%ebx

	call	leave_protected_mode
	.code16

	movb	$0xe8, %ah
        movb	$0x01, %al
	int	$0x15

	DATA32	call enter_protected_mode
	.code32

	movl	$0xFFFFFFFF, %eax
	cmpb	$0x86, %bh
	je	not_big

	andl	$0xFFFF, %edx	// clear sign extend 
	shll	$6, %edx	// and go from 64k to 1k chunks 
	movl	%edx, %eax	// store extended memory size 
	andl	$0xFFFF, %ecx	// clear sign extend 
 	addl	%ecx, %eax	// and add lower memory into 
						 

not_big:
	pop	%ebx
	pop	%ebp
	ret


boot_kernel_ak:

	movl	$0x2BADB002, %eax
	movl	0x8(%esp), %ebx

	/* boot kernel here (absolute address call) */
	call	0x4(%esp)
loop :
      jmp loop
       

boot_linux_ak :

	movl	linux_base_ptr, %ebx
	
	/* change %ebx to the segment address */
	shrl	$4, %ebx
	movl	%ebx, %eax
	addl	$0x20, %eax
	
			


	/* final setup for linux boot */

	call	leave_protected_mode
	.code16

	/* final setup for linux boot */
	cli
	movw	%bx, %ss
	movw	$0x9800, %sp
	
	movw	%bx, %ds
	movw	%bx, %es
	movw	%bx, %fs
	movw	%bx, %gs

       ljmp $(0x9020),$(0x0000)

#endif

