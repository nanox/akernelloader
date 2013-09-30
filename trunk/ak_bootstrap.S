/* Copyright (C) 2007 akernelloader TEAM
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


/* AKeL INITIAL PROGRAM LOADER */


TEXT__:.text
 
# This code run on real mode 16B
  .code16

.set ak_buffer_seg,   0x7000
.set STK_SEG_ak__,    0x2000   /* Stack base address */
 
.globl _main_ak__ ;

_main_ak__:
         /*jump far*/
        ljmp    $0, $(AKeL_START)
AKeL_START:

  CLI                  # Clear interrupt flag 
                       # interrupts disabled when interrupt flag cleared
  xorw  %ax,%ax        # Set to zero 
  movw  %ax,%ss        # Set to zero
  movw  %ax,%bx        # Set to zero
  xorw  %ax,%sp        # Stack offset 
  movw  %ax,%ds        # Data seg
  movw  $STK_SEG_ak__,%bx
  movw  %bx,%ss        # Stack section 
  STI                  # Set Interrupts flag 
  push %dx        # Driver Number
            
  call    CLS_ak__    # Clear screen
       

    pop %dx
    movb  %dl,(driver_num)    # The BIOS save the driver number in reg DL 

driver_type:

    call    return_dtype  # Load drive number
    cmp     $0x0,%dl      # Is floppy ? or bugus ?
    je      load_floppy    
    cmp     $0x1,%dl    
    je      load_floppy
    
    cmp     $0x80,%dl
    je      load_disk
    cmp     $0x81,%dl    
    je          load_disk

    
# Notify BIOS that boot failed. 
fatal_ak__ : 
        int $0x18
        
# Action : verify LBA. 
# DL == Driver Number
# BX == 0x55AA LBA is supported .  
extend_ak__:
      
  and    $0,%dx 
  xorw   %dx,%ax
  call   return_dtype    # Load drive number
  movw   $0x55aa,%bx
  movb   $0x41,%ah
  int    $0x13        
  ret            

disk_addr_chs:
    
  movw    $ak_buffer_seg ,%bx    # Buffer
  movw    %bx,%es    
  movw    $0,%bx
  movw    %bx,%cx      # Track 
  movw    %bx,%dx      # Set zero
  movb    $0x02,%cl    # Sector 
  movw    $0x0218,%ax  # Bios function to read and (AL) read 18 sectors
  ret
    
load_disk:
 
  call  extend_ak__
  cmpw  $0xaa55,%bx
  je    LBA_ak__
    
  call   return_dtype     # Return the drive type
  movb   $8,%ah           # Read driver parameters
  int    $0x13            # Call this
    
/*    
 - Verify ECHS :
     
         15            7         0
         -------------------------                      
         |   ch  high |  cl low  |
         -------------------------
            bit(8-9)       bit(6-7)  
        Bios 504MB limitation 
        1023 * 255 * 63 == Max. CX 

*/

 /* make sure that not use LBA */
  cmp  $0xFF,%dh     /*255 ?*/
  jne  read_disk
ECHS:
  cmp  $0xFFFF,%cx /* 111111.. ? */
  je   LBA_ak__
 
      
# Continue CHS mode
read_disk:

 
  call  disk_addr_chs
  call  return_dtype
  movb  $0x00,%dh     # Head  
  int   $0x13
  jc    reboot_ak__   # Carry != 0 
  jmp   new_start     # Jump to prepare new start

load_floppy:


  movw   $0x00,%ax  # First reset
  int    $0x13 
  jc     load_disk  # USB ? load it          
  call   return_dtype
  call   disk_addr_chs
  movb   $0x00,%dh    # Head  
  int    $0x13
  jmp    new_start    # Jump to prepare new start
    



          


# Logical Address      
LBA_ak__:

  push %ds
  call    return_dtype

  movw    $(DAP_ak__),%si  # Pack address
    
  xorl     %ebx,%ebx
     
  movl    %ebx,0xc(%si)
  movw    $0x3C,0x2(%si)         # Transfer 60 Blocks
  movw    $ak_buffer_seg,0x6(%si)     # Transfer here
  movw    %bx,0x4(%si)             # ZERO
  movl    (ak_kernel_sector),%ebx  # temp buffer
  movl    %ebx,0x8(%si)
  movb    $0x42,%ah     # Extended read
  int     $0x13
  pop %ds

new_start :
      
    movw    $ak_buffer_seg, %bx
    movw    (ak_kernel_segment),%ax 
    movw    %ax,%es

    pusha
    pushw    %ds
    
    movw    $0x3C00, %cx
    movw    %bx, %ds
    xorw    %si, %si
    xorw    %di, %di
    
    cld
    
    rep
    movsw

    popw    %ds
    popa

        call return_dtype
        movb %dl,(0x8000) /* Send driver number */
        

    /* Akernelloader kernel segment */
    jmpl    $(0x0000),$(0x8004)


getkey_ak__:
# e.g press any key to continue .. and continue ..
    movb $0x00,%al        # Set zero
    movb $0x00,%ah    
    int  $0x16        # Comunicate with the keybord
    ret            # Return
    
    
newline_ak__:    
        add    $1,pos     # New line ..
    mov    pos,%dh
    xorb   %dl,%dl
    call   cursor_pos_ak__
    sub    $1,pos     # Set zero
    ret
    
# Routine: print_ak__                        
# Action : Video services    
print_ak__:
 

    movb (%si),%al  # Move char in al
    inc  %si        # source i++ 
    movb $0x00,%bl  # Color background
    movb $0x00,%bh 
    movb $0xe,%ah  # Write chateracter
    int  $0x10     # Video Services
    cmpb $0x00,%al    # Compare
        jne  print_ak__    # If not zero jump
    call newline_ak__
    xorw  %si,%si
    ret             # Return

# Routine: cursor_pos_ak__                        
# Action : Set Cursor Position    
# DH = Row. DL = Column. BH = Page Number 
cursor_pos_ak__:
    movb  $0x02,%ah
    int   $0x10
    ret
    

CLS_ak__:
# Clear screen 

    movb    $0x00,%bh
    xorw    %dx,%dx
    call    cursor_pos_ak__

    # Write spaces to the entire screen
    movb    $0x09,%ah
    movb    $0x20,%al
    movw    $0x07,%bx
    movw    $0x2000,%cx
        int    $0x10

    call  cursor_pos_ak__
        ret

return_dtype:
      and  $0,%dx        # Set zero
      movb (driver_num),%dl    # Load drive number
      ret
      

 
reboot_ak__:
    call    CLS_ak__
    xor     %ax,%ax
    movw    $msg_press_to_reboot,%si
    call    print_ak__
    call    getkey_ak__
    int     $0x19




/***************************
*     DATA SEGMENT       *
****************************/
DATA__: .data

msg_press_to_reboot:     .ascii "ERROR Press any key to reboot"   
.byte 0
msg_jmp:     .ascii "Welcome to akernelloader. Press any key "   
.byte 0

#Device numbe retuned to bios
driver_num :      .byte    0 /* Save drive number    */
#Save the cursor position
pos :             .byte    0 

#  Disk address packet
DAP_ak__:
pack_size:              .byte    0x10  /* Packet size 16 bytes  0 */                      
reserv:            .byte   0x00  /* reserved  1 */                             
num_of_blk:        .word    0  /* Number of blocks to transfer and reserved 2 */
buffer_off :       .word    0  /* Buffer Adress to transfer offset  4 */ 
buffer_seg :       .word    0  /* Buffer Adress to transfer segment 6 */
logical_address:   .long    0  /* Logical block address  8 */
logical_extra:     .long    0  /* extra  12*/


ak_kernel_sector: .long    1

ak_kernel_segment: .word    0x0800


