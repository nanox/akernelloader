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

#ifndef _BIOS_AK_H_
#define _BIOS_AK_H_ 

#include <types_ak.h>


#define AK_BUFFER_ADDRESS 0x70000
#define AK_BUFFERSEG   0x7000

#define SECTOR_SIZE 512




/* These are defined in abios_ak.S, and declare the
   prototypes here.  */

/* Bios disk */

int driver_bios ;

#define BIOS_READ   0x00 
#define BIOS_WRITE  0x01 

#define BIOS_MEM_LOWER   0x00 
#define BIOS_MEM_UPPER   0x01

extern int bios_rw_chs_ak(int rw, int drive, int cylinder, int head, int sector, 
                            int num_sec, int segment);
extern int bios_rw_lba_ak(int rw, int drive, void *dap);
extern int get_drive_param_ak(int drive, unsigned long *cylinders, 
                    unsigned long *heads, unsigned long *sectors);

extern void getkey_ak(void);
extern void cls_ak(void);


struct disk_address_packet
      {
    unsigned char length;
    unsigned char reserved;
    unsigned short blocks;
    unsigned long buffer;
    unsigned long long block;
      } __attribute__ ((packed)) dap;

/* Drive parameters */
struct drv_parameters
{
  /* The number of cylinders */
  unsigned long cylinders;
  /* The number of heads */
  unsigned long heads;
  /* The number of sectors */
  unsigned long sectors;
  /* The total number of sectors */
  unsigned long total_sectors;
  /* Device sector size */
  unsigned long sector_size;
};

/* bios_ak.c */
int bios_disk_ak(int rw, int drive, uint64 sector, int nsec, int segment);
int get_parameters (int drive, struct drv_parameters *parameters);
/* memory probe routines */
int bios_memory_ak(int type);
int bios_bigmem_ak(void);
/* booting a multiboot executable */
void boot_kernel_ak(int entry, int mbinfo) __attribute__ ((noreturn));

void A20_line(int linear);


#endif /* End bios_ak.h */
