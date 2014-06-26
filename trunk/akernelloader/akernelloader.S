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

/* Akernelloader kernel 
   - Switching from real mode to protected mode.
   - This code was tested on a real machine intel pentium dual-core and AMD 64
*/	
save_driver: .long 0

.section .text
.code16

 .globl	 start_ak__, driver_bios

start_ak__:

          cli		

	/* set up %ds, %ss, and %es */
	xorw	%ax, %ax
	movw	%ax, %ds
	movw	%ax, %ss
	movw	%ax, %es


	/* set up the real mode stack */
	movl	$0x2000, %ebp
	movl	%ebp, %esp

	sti
		 
     
        call	enter_protected_mode
        .code32

       movl (save_driver),%eax
       movl %eax,(driver_bios) /* Save the driver number*/

	call	start
     
	hlt
 



