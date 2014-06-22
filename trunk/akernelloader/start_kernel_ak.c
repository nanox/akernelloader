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

#include <stdio_ak.h>
#include <bios_ak.h>
#include <multiboot_ak.h>
#include <elf_ak.h>
#include <disk_ak.h>
#include <os_ak/linux_ak.h>
#include <os_ak/freebsd.h>
#include <string_ak.h>
#include <fs_ak.h>

/* Simplify declaration of entry_addr. */
typedef void (*entry_func) (int, int, int, int, int, int)
     __attribute__ ((noreturn));

entry_func entry_addr;

static struct multiboot_mod_list module_list[MULTIBOOT_MODULE_NUM];
static int mod_cur_addr;


int multiboot_probe_ak(void* kernel_address, __u32 *mb_offset)
                                                                       {

        int offset;
        uint32 checksum;
        struct multiboot_header *hdr_ak;

        /* Scan through first 8kB of image file */
         
        for ( offset = 0 ; offset < MULTIBOOT_SEARCH  ; offset++) {
               
               hdr_ak = ((struct multiboot_header *) (kernel_address + offset));
                /* Check signature */
                if ( hdr_ak->magic == MULTIBOOT_HEADER_MAGIC ){
                        
                *mb_offset = kernel_address + offset;

                checksum = ( hdr_ak->magic + hdr_ak->flags +hdr_ak->checksum );
                if ( checksum == 0 )  
                        return 1;
                }
                
        }

        /* No multiboot header found */
        return 0;
}




int start_kernel_ak(void* kernel_address, const char * kernel_path ){



struct multiboot_header *hdr_ak;
__u32  entry , mb_offset,mem_tmp  ;
int bread ;

__u32 kernel_size;


struct linux_kernel_header *linux_h;

/* Set the boot loader name.  */
  mbinfo.boot_loader_name = (unsigned long) "Akernelloader";

 /* Get memory size */ 
  mbinfo.mem_lower = bios_memory_ak(BIOS_MEM_LOWER);
  mbinfo.mem_upper = bios_memory_ak(BIOS_MEM_UPPER);

  if((mem_tmp = bios_bigmem_ak()) != -1)  
  mbinfo.mem_upper = mem_tmp;



   FILE *fd;

   fd = fopen_ak(kernel_path); 
  
   if(fd == NULL)
       printf_ak("Can not open file, %s \n", kernel_path );

    kernel_size =  fd->filelength;
     
            printf_ak("\n Kernel path  %s \n", kernel_path ); 
  bread = fread_ak(boot_header ,MULTIBOOT_SEARCH, 1, fd );
  fs_op_ak.close(fd);
  printf_ak("bytes read: %i \n", bread);
  
  mbinfo.flags = (MULTIBOOT_INFO_MEMORY | MULTIBOOT_INFO_BOOT_LOADER_NAME);

   if (!multiboot_probe_ak(boot_header,&mb_offset )){
        printf_ak("Kernel multiboot header not found ..\n");

        linux_h = (struct linux_kernel_header *) boot_header ;
        // if is linux kernel loade it
        if (linux_h->boot_flag == 0xAA55 ){
            load_linux( kernel_size, kernel_path);
            return 1;
            }
          fd = fopen_ak(kernel_path);
          fread_ak((void*)kernel_address , kernel_size , 1, fd);
           
           if(IS_ELF(kernel_address)){
           printf_ak("\nIS ELF ...\nStarting kernel ...\n"); 
           load_elf(kernel_address,&entry);
           goto *(entry); 
         }else goto *(kernel_address);


  }
       
   fd = fopen_ak(kernel_path);
  int read = fread_ak((void*)kernel_address , kernel_size , 1, fd);


      /* Pointer to multiboot header */
       hdr_ak = ((struct multiboot_header *) (mb_offset));

        if ( ! (hdr_ak->flags & MULTIBOOT_FLAG_RAW  ) ) 
         /* Record execution entry point */
         entry = hdr_ak->entry_addr;

       if(IS_ELF(kernel_address))  
           load_elf(kernel_address,&entry);
         
        int i;
        for(i=0;i < MULTIBOOT_MODULE_NUM;i++){
              if( strlen_akel (module_path[i]) == 0)
                  break;
              if(i == 0)
                mod_cur_addr = entry + kernel_size ;
 
              

               mb_load_module (module_path[i]);

          }
       
           boot_kernel_ak ((int)entry, (int)&mbinfo ); 
         
}

int mb_load_module (char *module)
{
  int len;

  /* if we are supposed to load on 4K boundaries */
  mod_cur_addr = (mod_cur_addr + 0xFFF) & 0xFFFFF000;

   struct FILE *fd;

   fd = fopen_ak(module);
  
   if(fd == NULL)
       printf_ak("\n Can not open module file ...\n");
 
  len = len = fread_ak( mod_cur_addr,fd->filelength, 1, fd );
  if (! len)
    {
        fs_op_ak.close(fd);
      return 0;
    }

  printf_ak("   [Multiboot-module @ 0x%x, 0x%x bytes]\n", mod_cur_addr, len);

  /* these two simply need to be set if any modules are loaded at all */
  mbinfo.flags |= MULTIBOOT_INFO_MODS ;
  mbinfo.mods_addr = (int) module_list;
  module_list[mbinfo.mods_count].mod_start = mod_cur_addr;
  mod_cur_addr += len;
  module_list[mbinfo.mods_count].mod_end = mod_cur_addr;
  module_list[mbinfo.mods_count].pad = 0;

  /* increment number of modules included */
  mbinfo.mods_count++;

   fs_op_ak.close(fd);
  return 1;
}



int load_linux(__u32 kernel_size,const char * kernel_path){

   struct linux_kernel_header *linux_h;
   __u32 setup_size , linux_start_addr;
   int bread ;

    printf_ak("Read kernel pleas wait ... \n" );

   FILE *fd;
   fd = fopen_ak(kernel_path); 
  
   if(fd == NULL)
       printf_ak("Can not open file, %s \n", kernel_path );
     
  linux_h = (struct linux_kernel_header *) boot_header ;
  linux_h->type_of_loader= 0x70 ;
 int setup_sects = linux_h->setup_sects;
 char * cmd_line= COMMAND_LINE_OFFSET;

    /* If sectors setup are not defined , set it to the default (4).  */
      if (! setup_sects)
    setup_sects = 4 ;

          setup_size = (setup_sects + 1) * 512 ;
         
      linux_h->cl_magic = LINUX_CL_MAGIC;
      linux_h->cl_offset = cmd_line - linux_base_ptr;
      kernel_size -= setup_size;

         
          linux_start_addr = (char *) LINUX_KERNEL_ADDR ;      
          linux_base_ptr  = LINUX_SETUP_ADDR;
          
          printf_ak ("Setup size in bytes    %i \n",setup_size); 
          printf_ak ("Kernel size in bytes   %i \n",kernel_size);
          printf_ak ("Load kernel please wait ...\n");

             bread = fread_ak((void*)linux_base_ptr , setup_size , 1, fd );  
             printf_ak ("Setup bytes read %i \n",bread );
             fclose_ak(fd);

           fd = fopen_ak(kernel_path); 
           fseek_ak(fd, setup_size,SEEK_SET);
           bread = fread_ak((void*)linux_start_addr , kernel_size , 1, fd );  
           printf_ak ("Kernel bytes read %i \n",bread );
         
    
     memset_akel(LINUX_SETUP_ADDR + (setup_sects+1)*512, 0,
                      (64-(setup_sects+1))*512);

    memmove_akel( LINUX_SETUP_ADDR ,boot_header,MULTIBOOT_SEARCH);
 
        return 1 ;

}


int load_initrd (const char *initrd_path)
{
  
  unsigned long addr_initrd, len ;
  int intrd_size;
  unsigned long initrd_max, linux_mem_size ;
  struct linux_kernel_header *linux_h = (struct linux_kernel_header *) LINUX_SETUP_ADDR ;
 
  struct FILE *fd;

   fd = fopen_ak(initrd_path);
  
   if(fd == NULL)
       printf_ak("\n Can not open ramdisk file ...\n");
    intrd_size = fd->filelength;
     
  initrd_max = (linux_h->header == LINUX_MAGIC_SIGNATURE && linux_h->version >= 0x0203
          ? linux_h->initrd_addr_max : LINUX_INITRD_MAX_ADDRESS );
  
   

  linux_mem_size = mbinfo.mem_upper << 10;
    if(linux_mem_size  < initrd_max)
       initrd_max = linux_mem_size ;
 
   addr_initrd = (initrd_max - intrd_size) ;
   addr_initrd  &= ~0xfff; 

   addr_initrd -= 10000; 
   len = fread_ak( addr_initrd ,intrd_size, 1, fd );
 

   printf_ak(" Read ram disk pleas wait ... \n"  
            "Ram disk @ [ 0x%x ] , %u bytes]\n", addr_initrd, len );
   


linux_h->ramdisk_image = addr_initrd ;
linux_h->ramdisk_size = intrd_size ;
 
  
  return 0 ;
}





