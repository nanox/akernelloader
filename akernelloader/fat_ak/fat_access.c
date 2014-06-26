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
#include <partition_ak.h>
#include <fat/fat_access.h>
#include <fat/fat_fs.h>
#include <fat/fat_defs.h>
#include <fat/fat_table.h>
#include <fat/fat_misc.h>
#include <fat/fat_string.h>
#include <disk_ak.h>


int fat32_info_ak(struct fat_info_ak *fatinfo)
{

  unsigned char superblock[512];


 
   
  media_read_ak( partition.start, superblock , 512);

  fatinfo->lba_begin = partition.start ;

  fatinfo->bytes_per_sec = * (unsigned char *) &superblock[BPB_BYTSPERSEC];
  fatinfo->sec_per_clust = * (unsigned char *) &superblock[BPB_SECPERCLUS ];
  fatinfo->reserved_sectors = * (unsigned char *) &superblock[BPB_RSVDSECCNT];
  fatinfo->numfats = * (unsigned char*) &superblock[BPB_NUMFATS];
  fatinfo->root_entries = * (unsigned char *) &superblock[BPB_ROOTENTCNT ];
  fatinfo->total_sectors = * (unsigned char *) &superblock[BPB_TOTSEC16];
  
  if(fatinfo->total_sectors == 0) {
    fatinfo->total_sectors = * (unsigned int *) &superblock[BPB_TOTSEC32];
  }
  
  fatinfo->media = * (unsigned char *) &superblock[BPB_MEDIA];
  
  fatinfo->sec_per_track = * (unsigned short *) &superblock[BPB_SECPERTRK];
  fatinfo->num_heads = * (unsigned short *) &superblock[BPB_NUMHEADS];
  fatinfo->hidden_sectors = * (unsigned int *) &superblock[BPB_HIDDSEC];
  
  //fat32 specific
  fatinfo->fat_size = * (unsigned int *) &superblock[BPB_FAT32_FATSZ32];
  fatinfo->root_cluster = * (unsigned int *) &superblock[BPB_FAT32_ROOTCLUS];

  return 0;
 }

int fatfs_init(struct fatfs *fs, struct fat_info_ak *fatinfo )
{



    // Load Parameters of FAT partition     
    fs->sectors_per_cluster    = fatinfo->sec_per_clust ;
    fs->root_entry_count     = fatinfo->root_entries ;
    fs->fat_sectors     = fatinfo->fat_size;
    fs->rootdir_first_cluster = fatinfo->root_cluster;
    fs->reserved_sectors  = fatinfo->reserved_sectors;

    fs->lba_begin = fatinfo->lba_begin;
    fs->fat_begin_lba = fs->lba_begin + fatinfo->reserved_sectors;

    // Root directory region start  
    fs->rootdir_first_sector = fs->fat_begin_lba + (fatinfo->numfats * fs->fat_sectors);
    // Root directory region size 
    fs->rootdir_sectors = (fs->root_entry_count * 32) / 512 ;

    
    

    // The address of the first data cluster on this volume
     fs->cluster_begin_lba =  fs->rootdir_first_sector + (fs->root_entry_count * 32) / 512 ;
     fs->data_start_sec = fatinfo->reserved_sectors + fatinfo->fat_size * fatinfo->numfats;
     fs->cluster_size = 512 * fatinfo->sec_per_clust;
 
        return 0 ;    
}

uint32 fatfs_lba_of_cluster(struct fatfs *fs, uint32 Cluster_Number)
{
 
        return ((fs->cluster_begin_lba + ((Cluster_Number-2)*fs->sectors_per_cluster)));
}

uint32 fatfs_cluster_of_lba(struct fatfs *fs, uint32 lba){

      return ((lba-fs->cluster_begin_lba )/fs->sectors_per_cluster)+2;

}


uint32 fatfs_get_root_cluster(struct fatfs *fs)
{
    // NOTE: On FAT16 this will be 0 which has a special meaning...
    return fs->rootdir_first_cluster;
}



int fatfs_sector_reader(struct fatfs *fs, uint32 start_cluster, uint32 offset, uint8 *target)
{
    uint32 sector_to_read = 0;
    uint32 cluster_to_read = 0;
    uint32 cluster_chain = 0;
    uint32 i;
    uint32 lba;

    
        // Set start of cluster chain to initial value
        cluster_chain = start_cluster;

        // Find parameters
        cluster_to_read = offset / fs->sectors_per_cluster;      
        sector_to_read = offset - (cluster_to_read*fs->sectors_per_cluster);

        // Follow chain to find cluster to read
        for (i=0; i<cluster_to_read; i++)
            cluster_chain = fatfs_find_next_cluster(fs, cluster_chain);

        // If end of cluster chain then return false
        if (cluster_chain == FAT32_LAST_CLUSTER) 
            return 0;

        // Calculate sector address
        lba = fatfs_lba_of_cluster(fs, cluster_chain)+sector_to_read;
    

    // User provided target array
    if (target)
        return media_read_ak(lba, target, 512);
    // Else read sector if not already loaded
    else if (lba != fs->currentsector.address)
    {
        fs->currentsector.address = lba;
        return media_read_ak(fs->currentsector.address, fs->currentsector.sector, 512);
    }
    else
        return 1;
}


uint32 fatfs_get_file_entry(struct fatfs *fs, uint32 Cluster, char *name_to_find, struct fat_dir_entry *sfEntry)
{
    uint8 item=0;
    uint16 recordoffset = 0;
    uint8 i=0;
    int x=0;
    char *long_filename = NULL;
    char short_filename[13];
    struct lfn_cache lfn;
    int dotRequired = 0;
    struct fat_dir_entry *directoryEntry;

    fatfs_lfn_cache_init(&lfn, 1);

    // Main cluster following loop
    while (1)
    {
        // Read sector
        if (fatfs_sector_reader(fs, Cluster, x++, 0)) // If sector read was successfull
        {
            // Analyse Sector
            for (item = 0; item < FAT_DIR_ENTRIES_PER_SECTOR; item++)
            {
                // Create the multiplier for sector access
                recordoffset = FAT_DIR_ENTRY_SIZE * item;

                // Overlay directory entry over buffer
                directoryEntry = (struct fat_dir_entry*)(fs->currentsector.sector+recordoffset);

                
#if FATFS_INC_LFN_SUPPORT
                
                // Long File Name Text Found
                if (fatfs_entry_lfn_text(directoryEntry) ) 
                    fatfs_lfn_cache_entry(&lfn, fs->currentsector.sector+recordoffset);

                // If Invalid record found delete any long file name information collated
                else if (fatfs_entry_lfn_invalid(directoryEntry) ) 
                    fatfs_lfn_cache_init(&lfn, 0);

                // Normal SFN Entry and Long text exists 
                else if (fatfs_entry_lfn_exists(&lfn, directoryEntry) ) 
                {
                    long_filename = fatfs_lfn_cache_get(&lfn);

                    // Compare names to see if they match
                    if (fatfs_compare_names(long_filename, name_to_find)) 
                    {
                     
                        memmove_akel(sfEntry,directoryEntry,sizeof(struct fat_dir_entry));
                        return 1;
                    }

                    fatfs_lfn_cache_init(&lfn, 0);
                }                 
                else 
#endif
                // Normal Entry, only 8.3 Text    
                if (fatfs_entry_sfn_only(directoryEntry) )
                {
                    memset_akel(short_filename, 0, sizeof(short_filename));

                    // Copy name to string
                    for (i=0; i<8; i++) 
                        short_filename[i] = directoryEntry->Name[i];

                    // Extension
                    dotRequired = 0;
                    for (i=8; i<11; i++) 
                    {
                        short_filename[i+1] = directoryEntry->Name[i];
                        if (directoryEntry->Name[i] != ' ')
                            dotRequired = 1;
                    }

                    // Dot only required if extension present
                    if (dotRequired)
                    {
                        // If not . or .. entry
                        if (short_filename[0]!='.')
                            short_filename[8] = '.';
                        else
                            short_filename[8] = ' ';
                    }
                    else
                        short_filename[8] = ' ';
                    
                    // Compare names to see if they match
                    if (fatfs_compare_names(short_filename, name_to_find)) 
                    {
                        memmove_akel(sfEntry,directoryEntry,sizeof(struct fat_dir_entry));
                        return 1;
                    }

                    fatfs_lfn_cache_init(&lfn, 0);
                }
            } // End of if
        } 
        else
            break;
    } // End of while loop

    return 0;
}


