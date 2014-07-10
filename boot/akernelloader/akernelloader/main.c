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
#include <partition_ak.h>
#include <bios_ak.h>
#include <elf_ak.h>
#include <config_ak.h>
#include <fs_ak.h>

extern void start(void);


void start(){

A20_line(1);

cls_ak();

printf_ak("\n_.: Welcome to Akernelloader :._\n\n");
printf_ak("Load GDT.................. [OK]\n");
printf_ak("Run on protected mode .... [OK]\n");
printf_ak("Switch modes ............. [OK]\n\n");
printf_ak("Find a bootable partition ...\n");
if(open_partition()){     
     printf_ak("Partition type id: %x  \n", partition.type );
     printf_ak("Partition start  : %lu \n", partition.start);
     printf_ak("Number of sectors in partition : %lu \n", partition.length );}
else{
      printf_ak("Bad file system or boot blag is not set\n");
      getkey_ak();
    }
    
printf_ak("\n\nGet memory size ...\n");
  unsigned long mem_upper ,mem_tmp , mem_lower ;
  mem_lower = bios_memory_ak(BIOS_MEM_LOWER);
  mem_upper = bios_memory_ak(BIOS_MEM_UPPER);


if((mem_tmp = bios_bigmem_ak()) != -1)  
    mem_upper = mem_tmp;
 
   printf_ak("Memory lower: %u KB\n", mem_lower );
   printf_ak("Memory upper: %u KB\n", mem_upper );

printf_ak("\n\nFind driver parameters ...\n");

struct drv_parameters parameters;
get_parameters (driver_bios, &parameters);

  printf_ak("The number of cylinders: %lu \n",parameters.cylinders);
  printf_ak("The number of heads: %lu \n",parameters.heads);
  printf_ak("The number of sectors : %lu \n",parameters.sectors);
  printf_ak("The total number of sectors: %lu \n",parameters.total_sectors);
  printf_ak("Device sector size: %lu \n",parameters.sector_size);

/*To define the size, address and path 
    of the kernel edit the config_ak.h */

 unsigned char *addr =(unsigned char*) kernel_address;


  init_file_op(partition.type);
  
  //Read the configuration file 
  read_fconfig(config_path);

  //Graphical menu
   run_menu_akernelloder();


printf_ak("Set device ... %s\n",device_str);
     set_device_ak (device_str);

/*Uses new device configuration*/
 open_partition();
 init_file_op(partition.type);     

    //Find the kernel header
 if(start_kernel_ak(addr, kernel_path )){
   load_initrd (initrd_path);
   printf_ak("Starting Linux kernel\n"
             "Please wait ...\n");
   boot_linux_ak();
 }

 getkey_ak();

}




