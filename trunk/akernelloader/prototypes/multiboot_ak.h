 
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

#ifndef _MULTIBOOT_AK_H
#define _MULTIBOOT_AK_H 


#include <types_ak.h>

/* How many bytes from the start of the file we search for the header. */
     #define MULTIBOOT_SEARCH                        8192
     
     /* The magic field should contain this. */
     #define MULTIBOOT_HEADER_MAGIC                  0x1BADB002
     
     /* This should be in %eax. */
     #define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002
     
     /* The bits in the required part of flags field we don't support. */
     #define MULTIBOOT_UNSUPPORTED                   0x0000fffc
     
     /* Alignment of multiboot modules. */
     #define MULTIBOOT_MOD_ALIGN                     0x00001000
     
     /* Alignment of the multiboot info structure. */
     #define MULTIBOOT_INFO_ALIGN                    0x00000004
     
     /* Flags set in the 'flags' member of the multiboot header. */
     
     /* Align all boot modules on i386 page (4KB) boundaries. */
     #define MULTIBOOT_PAGE_ALIGN                    0x00000001
     
     /* Must pass memory information to OS. */
     #define MULTIBOOT_MEMORY_INFO                   0x00000002
     
     /* Must pass video information to OS. */
     #define MULTIBOOT_VIDEO_MODE                    0x00000004
     
     /* This flag indicates the use of the address fields in the header
        if the image is not multiboot elf. */
     #define MULTIBOOT_FLAG_RAW                  0x00010000
     
     /* Flags to be set in the 'flags' member of the multiboot info structure. */
     
     /* is there basic lower/upper memory information? */
     #define MULTIBOOT_INFO_MEMORY                   0x00000001
     /* is there a boot device set? */
     #define MULTIBOOT_INFO_BOOTDEV                  0x00000002
     /* is the command-line defined? */
     #define MULTIBOOT_INFO_CMDLINE                  0x00000004
     /* are there modules to do something with? */
     #define MULTIBOOT_INFO_MODS                     0x00000008
     
     /* These next two are mutually exclusive */
     
     /* is there a symbol table loaded? */
     #define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
     /* is there an ELF section header table? */
     #define MULTIBOOT_INFO_ELF_SHDR                 0X00000020
     
     /* is there a full memory map? */
     #define MULTIBOOT_INFO_MEM_MAP                  0x00000040
     
     /* Is there drive info? */
     #define MULTIBOOT_INFO_DRIVE_INFO               0x00000080
     
     /* Is there a config table? */
     #define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100
     
     /* Is there a boot loader name? */
     #define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200
     
     /* Is there a APM table? */
     #define MULTIBOOT_INFO_APM_TABLE                0x00000400
     
     /* Is there video information? */
     #define MULTIBOOT_INFO_VIDEO_INFO               0x00000800
        
     #define MULTIBOOT_MODULE_NUM                    99
     struct multiboot_header
     {
       /* Must be MULTIBOOT_MAGIC - see above. */
       __u32 magic;
     
       /* Feature flags. */
       __u32 flags;
     
       /* The above fields plus this one must equal 0 mod 2^32. */
       __u32 checksum;
     
       /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
       __u32 header_addr;
       __u32 load_addr;
       __u32 load_end_addr;
       __u32 bss_end_addr;
       __u32 entry_addr;
     
       /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
       __u32 mode_type;
       __u32 width;
       __u32 height;
       __u32 depth;
     };
     
     /* The symbol table for a.out. */
     struct multiboot_aout_symbol_table
     {
       __u32 tabsize;
       __u32 strsize;
       __u32 addr;
       __u32 reserved;
     };
     typedef struct multiboot_aout_symbol_table multiboot_aout_symbol_table_t;
     
     /* The section header table for ELF. */
     struct multiboot_elf_section_header_table
     {
       __u32 num;
       __u32 size;
       __u32 addr;
       __u32 shndx;
     };
     typedef struct multiboot_elf_section_header_table multiboot_elf_section_header_table_t;
     
     struct multiboot_info
     {
       /* Multiboot info version number */
       __u32 flags;
     
       /* Available memory from BIOS */
       __u32 mem_lower;
       __u32 mem_upper;
     
       /* "root" partition */
       __u32 boot_device;
     
       /* Kernel command line */
       __u32 cmdline;
     
       /* Boot-Module list */
       __u32 mods_count;
       __u32 mods_addr;
     
       union
       {
         multiboot_aout_symbol_table_t aout_sym;
         multiboot_elf_section_header_table_t elf_sec;
       } u;
     
       /* Memory Mapping buffer */
       __u32 mmap_length;
       __u32 mmap_addr;
     
       /* Drive Info buffer */
       __u32 drives_length;
       __u32 drives_addr;
     
       /* ROM configuration table */
       __u32 config_table;
     
       /* Boot Loader Name */
       __u32 boot_loader_name;
     
       /* APM table */
       __u32 apm_table;
     
       /* Video */
       __u32 vbe_control_info;
       __u32 vbe_mode_info;
       __u16 vbe_mode;
       __u16 vbe_interface_seg;
       __u16 vbe_interface_off;
       __u16 vbe_interface_len;
     };
     typedef struct multiboot_info multiboot_info_t;
     
     struct multiboot_mmap_entry
     {
       __u32 size;
       __u64 addr;
       __u64 len;
     #define MULTIBOOT_MEMORY_AVAILABLE              1
     #define MULTIBOOT_MEMORY_RESERVED               2
       __u32 type;
     } __attribute__((packed));
     typedef struct multiboot_mmap_entry multiboot_memory_map_t;
     
     struct multiboot_mod_list
     {
       /* the memory used goes from bytes 'mod_start' to 'mod_end-1' inclusive */
       __u32 mod_start;
       __u32 mod_end;
     
       /* Module command line */
       __u32 cmdline;
     
       /* padding to take it to 16 bytes (must be zero) */
       __u32 pad;
     };
     
     
struct multiboot_info mbinfo;

unsigned char boot_header[MULTIBOOT_SEARCH];
char module_path[MULTIBOOT_MODULE_NUM][1000];
    
#endif /* END _MULTIBOOT_AK_H */


