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

#ifndef _ACCESS_H_
#define _ACCESS_H_ 


#include "fat_defs.h"
#include "fat_opts.h"




//        Name                Offset
#define BPB_BYTSPERSEC          11    // Length = 2
#define BPB_SECPERCLUS          13    // Length = 1
#define BPB_RSVDSECCNT          14    // Length = 2
#define BPB_NUMFATS             16    // Length = 1
#define BPB_ROOTENTCNT          17    // Length = 2
#define BPB_TOTSEC16            19    // Length = 2
#define BPB_MEDIA               21    // Length = 1
#define BPB_SECPERTRK           24    // Length = 2
#define BPB_NUMHEADS            26    // Length = 2
#define BPB_HIDDSEC             28    // Length = 4
#define BPB_TOTSEC32            32    // Length = 4

// FAT 32
#define BPB_FAT32_FATSZ32       36    // Length = 4
#define BPB_FAT32_ROOTCLUS      44    // Length = 4


#define FAT32_LAST_CLUSTER              0xFFFFFFFF
#define FAT32_INVALID_CLUSTER           0xFFFFFFFF

#define FAT_DIR_ENTRIES_PER_SECTOR          (FAT_SECTOR_SIZE / FAT_DIR_ENTRY_SIZE)

struct fat_info_ak {
  uint8 bytes_per_sec;
  uint8 sec_per_clust;
  uint8 reserved_sectors;
  uint8 numfats;
  uint8 root_entries;
  uint32 total_sectors;
  uint8 media;
  uint32 fat_size;
  uint16 sec_per_track;
  uint16 num_heads;
  uint32 hidden_sectors;
  uint32 root_cluster;

  uint32 lba_begin ;
} ;


struct fat_buffer
{
    uint8                   sector[FAT_SECTOR_SIZE * FAT_BUFFER_SECTORS];
    uint32                  address; 
   
};


struct fatfs
{
    // Filesystem globals
    uint8	sectors_per_cluster;
    uint32	cluster_begin_lba;
    uint32	rootdir_first_cluster;
    uint32	rootdir_first_sector;
    uint32	rootdir_sectors;
    uint32	fat_begin_lba;
    uint16	fs_info_sector;
    uint32	lba_begin;
    uint32	fat_sectors;
    uint32	next_free_cluster;
    uint16	root_entry_count;
    uint16	reserved_sectors;
    uint8	num_of_fats;
    uint32	data_start_sec;
    uint32	cluster_size;
   

    // Working buffer
    struct fat_buffer        currentsector;
    
    
};

int fatfs_init(struct fatfs *fs, struct fat_info_ak *fatinfo );
int fatfs_sector_reader(struct fatfs *fs, uint32 start_cluster, uint32 offset, uint8 *target);
int fat32_info_ak(struct fat_info_ak *fatinfo);
uint32 fatfs_get_root_cluster(struct fatfs *fs);
uint32  fatfs_lba_of_cluster(struct fatfs *fs, uint32 Cluster_Number);
uint32 fatfs_cluster_of_lba(struct fatfs *fs, uint32 lba);
uint32  fatfs_get_file_entry(struct fatfs *fs, uint32 Cluster, char *nametofind, struct fat_dir_entry *sfEntry);


#endif


