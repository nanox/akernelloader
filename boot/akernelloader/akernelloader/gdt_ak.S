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

.text
.global  enter_protected_mode,leave_protected_mode;

enter_protected_mode:
	.code16
	cli

	/* load the GDT register */
	DATA32	ADDR32	lgdt reg_gdt_ak__

	/* turn on protected mode */
	movl	%cr0, %eax
	orl	$0x1, %eax
	movl	%eax, %cr0

	/* jump to relocation, flush prefetch queue, and reload %cs */
	DATA32	ljmp	$0x8, $pm_segment_ak__ /* Code segment */

	/*
	 *  The ".code32" directive only works in GAS, the GNU assembler!
	 *  This gets out of "16 bit" mode.
	 */
	.code32

pm_segment_ak__:
	/* reload other segment registers */
	movw	$0x10, %ax /* Data segment */
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	
	/* put the return address in a known safe location */
	movl	(%esp), %eax
	movl	%eax, 0x2000

	/* get protected mode stack */
	movl	pm_stack_ak__, %eax
	movl	%eax, %esp
	movl	%eax, %ebp

	/* get return address onto the right stack */
	movl	0x2000, %eax
	movl	%eax, (%esp)

	/* zero %eax */
	xorl	%eax, %eax
       
       ret


leave_protected_mode:
	/* just in case, set GDT */
	lgdt	reg_gdt_ak__

	/* save the protected mode stack */
	movl	%esp, %eax
	movl	%eax, pm_stack_ak__

	/* get the return address */
	movl	(%esp), %eax
	movl	%eax, 0x2000

	/* set up new stack */
	movl	$0x2000, %eax
	movl	%eax, %esp
	movl	%eax, %ebp

	/* set up segment limits */
	movw	$0x20, %ax /* Data segment */
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	/* this might be an extra step */
	ljmp	$0x18, $tmp	/* jump to a 16 bit code segment */

tmp:
	.code16

	/* clear the PE bit of CR0 */
	movl	%cr0, %eax
	andl 	$0xfffffffe , %eax
	movl	%eax, %cr0

	/* flush prefetch queue, reload %cs */
	DATA32	ljmp	$0, $rm_segment_ak__

rm_segment_ak__:

	/* we are in real mode now
	 * set up the real mode segment registers : DS, SS, ES
	 */
	/* zero %eax */
	xorl	%eax, %eax

	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	/* restore interrupts */
	sti

        ret
     
pm_stack_ak__:
	.long	0x68000

/*The Global Descriptor Table*/
gdt:
	.word	0, 0
	.byte	0, 0, 0, 0

	/* code segment */
	.word	0xFFFF, 0
	.byte	0, 0x9A, 0xCF, 0

	/* data segment */
	.word	0xFFFF, 0
	.byte	0, 0x92, 0xCF, 0

	/* 16 bit real mode CS */
	.word	0xFFFF, 0
	.byte	0, 0x9E, 0, 0

	/* 16 bit real mode DS */
	.word	0xFFFF, 0
	.byte	0, 0x92, 0, 0

.equ	limit_gdt, (. - gdt )-1	# GDT segment-limit 

/* this is the GDT descriptor */
reg_gdt_ak__:
	.word	limit_gdt			/* limit */
	.long	gdt			/* addr */

