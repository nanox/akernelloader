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

#ifndef _ELF_AK_H_
#define _ELF_AK_H_ 


#define EI_NIDENT 16
#define ET_EXEC   2
#define PT_LOAD   1

#include <types_ak.h>

/* 32-bit data types */

typedef unsigned long Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned long Elf32_Off;
typedef signed long Elf32_Sword;
typedef unsigned long Elf32_Word;

typedef struct { 
unsigned char e_ident[EI_NIDENT];
Elf32_Half  e_type;
Elf32_Half  e_machine;
Elf32_Word  e_version;
Elf32_Addr  e_entry;
Elf32_Off   e_phoff;
Elf32_Off   e_shoff;
Elf32_Word  e_flags;
Elf32_Half  e_ehsize;
Elf32_Half  e_phentsize;
Elf32_Half  e_phnum;
Elf32_Half  e_shentsize;
Elf32_Half  e_shnum;
Elf32_Half  e_shstrndx;

} Elf32_Ehdr; 

typedef struct
{
  Elf32_Word	p_type;			/* Segment type */
  Elf32_Off	p_offset;		/* Segment file offset */
  Elf32_Addr	p_vaddr;		/* Segment virtual address */
  Elf32_Addr	p_paddr;		/* Segment physical address */
  Elf32_Word	p_filesz;		/* Segment size in file */
  Elf32_Word	p_memsz;		/* Segment size in memory */
  Elf32_Word	p_flags;		/* Segment flags */
  Elf32_Word	p_align;		/* Segment alignment */
} Elf32_Phdr;

int IS_ELF(void* address);
void load_elf(void* mem_addr,__u32 *entry);

#endif /* End elf_ak.h */

