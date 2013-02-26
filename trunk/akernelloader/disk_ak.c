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
#include <string_ak.h>
#include <bios_ak.h>
#include <disk_ak.h>
#include <types_ak.h>

int media_read_ak(uint64 sector,  void *buffer, size_t nbyte)
{
 

  uint32 i;
  void *bufaddr = (void *) AK_BUFFER_ADDRESS  ;
  uint32 sector_count = nbyte / 512 ;
  uint32 remainder = nbyte % 512 ;
 
 for (i=0;i<sector_count;i++)
    {
        if(sector_count == 0)
        break;
          
           if(bios_disk_ak( BIOS_READ, driver_bios , sector , 1 , AK_BUFFERSEG) != 0 ) 
              return 0 ;       
           memmove_akel (buffer, bufaddr, 512);
                  
        sector ++;
        buffer += 512;
    }

   if(remainder){
  
     if(bios_disk_ak( BIOS_READ, driver_bios , sector, 1 , AK_BUFFERSEG) != 0 ) 
              return 0 ;        
           memmove_akel (buffer, bufaddr, remainder);
   }

   return 1 ;
}

int supported_fs(int type)
{
    switch (type)
    {
        case 0x0b: return (1) ; /*FAT32 */
        case 0x0c: return (1) ; /*FAT32 */
        case 0x83: return (1); /*EXT2 ,EXT3 */
        default: return (0) ;
    }
}

int CheckEBR(__u32 ebr_lba)
{
    EXTENDED_BOOT_RECORD ebr;
    uint64 offset ,next_erb_lba = 0;
 
     

    do
    {    
          offset= (uint64) (ebr_lba + next_erb_lba) ;
        // Read and validate the extended boot record
         if(media_read_ak(offset ,&ebr, 512) == 0 )
               printf_ak("READ ERROR... \n");

        if(ebr.partition.block_count  > 0)
        {



          if( ebr.partition.status  == PARTITION_BOOTABLE ){
                 if(supported_fs(ebr.partition.type)){
                        
                 partition.type  = ebr.partition.type;
                 partition.start  = ebr_lba + next_erb_lba + ebr.partition.lba_start;
                 partition.length = ebr.partition.block_count;
                   
                  return 1;
                 }
          }
        // Get the start sector of the current partition
        // and the next extended boot record in the chain
        next_erb_lba = ebr.next_ebr.lba_start;

      }
   } while(next_erb_lba > 0);

       return 0;
}


int open_partition (void)
{


 int i = 0;

 MASTER_BOOT_RECORD mbr;


  /* Read the MBR or the boot sector of the extended partition.  */
  if(media_read_ak(0 ,&mbr, 512) == 0)
     printf_ak("READ ERROR... \n");

  /* Check if it is valid.  */
  if (mbr.signature != MBR_SIGNATURE)
      printf_ak("Partition table are not signed ..\n");
          


/* Search the boot flag in current table.  */

 for ( i; i < PARTITION_PRIM_MAX ; i++)
    {
        
        PARTITION_RECORD * part = (PARTITION_RECORD *) &mbr.partitions[i];

         /* Search the boot flag in the extended partition.  */
        if(part->type == PARTITION_TYPE_EXTENDED || part->type == PARTITION_TYPE_WIN95_EXTENDED)
        {
                        if ( CheckEBR(part->lba_start))
                            return 1;
                        
        }

        if(part->block_count > 0)
        {

            if(part->status == PARTITION_BOOTABLE ){
                 if(supported_fs(part->type)){
                        
                 partition.type  = part->type;
                 partition.start  = part->lba_start;
                 partition.length = part->block_count;
            
                  return 1;
                }

            }
     
      }
   
}
           return 0;

}

