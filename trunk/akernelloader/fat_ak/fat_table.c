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

#include <string_ak.h>
#include <fat/fat_defs.h>
#include <fat/fat_access.h>
#include <fat/fat_table.h>
#include <disk_ak.h>

uint32 fatfs_find_next_cluster(struct fatfs *fs, uint32 current_cluster)
{

  unsigned char FATBuffer[512];
  uint32 FAT_sector_offset, position;
  uint32 nextcluster;
  
#define GET_32BIT_WORD(buffer, location)    ( ((uint32)buffer[location+3]<<24) + ((uint32)buffer[location+2]<<16) + ((uint32)buffer[location+1]<<8) + (uint32)buffer[location+0] )
#define FAT32_GET_32BIT_WORD(location)  ( GET_32BIT_WORD(FATBuffer, location) )


        // Why is '..' labelled with cluster 0 when it should be 2 ??
        if (current_cluster==0) current_cluster=2;

        // Find which sector of FAT table to read
        FAT_sector_offset = current_cluster / 128;


 if (!media_read_ak(fs->fat_begin_lba + FAT_sector_offset , FATBuffer ,512))
          return (FAT32_LAST_CLUSTER);

        // Find 32 bit entry of current sector relating to cluster number 
        // 128 entry per sector
        position = (current_cluster % 128) * 4; 

        // Read Next Clusters value from Sector Buffer
        nextcluster = FAT32_GET_32BIT_WORD((uint16)position);    

        // Mask out MS 4 bits (its 28bit addressing)
        nextcluster = nextcluster & 0x0FFFFFFF;         

        // If 0x0FFFFFFF then end of chain found
        if (nextcluster==0x0FFFFFFF) 
                return (FAT32_LAST_CLUSTER); 
        else 
        // Else return next cluster
                return (nextcluster);

           
} 





