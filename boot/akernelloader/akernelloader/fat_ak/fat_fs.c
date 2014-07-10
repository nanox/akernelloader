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
#include <fat/fat_access.h>
#include <fat/fat_fs.h>
#include <fat/fat_defs.h>
#include <fat/fat_table.h>
#include <fat/fat_misc.h>
#include <fat/fat_string.h>
#include <disk_ak.h>
#include <partition_ak.h>
#include <fs_ak.h>

static struct fatfs       _fs;

void fat_init(void)
{     
   struct fat_info_ak fatinfo; 
   fat32_info_ak(&fatinfo);
   fatfs_init(&_fs,&fatinfo);
}

static int _open_directory(char *path, uint32 *pathCluster)
{
    int levels;
    int sublevel;
    char currentfolder[FATFS_MAX_LONG_FILENAME];
    struct fat_dir_entry sfEntry;
    uint32 startcluster;

    // Set starting cluster to root cluster
    startcluster = fatfs_get_root_cluster(&_fs);

    // Find number of levels
    levels = fatfs_total_path_levels(path);

    // Cycle through each level and get the start sector
    for (sublevel=0;sublevel<(levels+1);sublevel++) 
    {
        if (fatfs_get_substring(path, sublevel, currentfolder, sizeof(currentfolder)) == -1)
            return 0;

        // Find clusteraddress for folder (currentfolder) 
        if (fatfs_get_file_entry(&_fs, startcluster, currentfolder,&sfEntry))
        {
            // Check entry is folder
            if (fatfs_entry_is_dir(&sfEntry))
                startcluster = ((FAT_HTONS((uint32)sfEntry.FstClusHI))<<16) + FAT_HTONS(sfEntry.FstClusLO);
                
            else
                return 0;
        }
        else
            return 0;
    }

    *pathCluster = startcluster;
    return 1;
}


struct FILE* fat_fopen(const char *path)
{
    FILE* file; 
    struct fat_dir_entry sfEntry;

    // Allocate a new file handle
    file = (struct FILE *) &_file;
  
    // Clear filename
    memset_akel(file->path, '\0', sizeof(file->path));
    memset_akel(file->filename, '\0', sizeof(file->filename));

    // Split full path into filename and directory path
    if (fatfs_split_path((char*)path, file->path, sizeof(file->path), file->filename, sizeof(file->filename)) == -1) 
        return NULL;
   
    // If file is in the root dir
    if (file->path[0]==0)
        file->parentcluster = fatfs_get_root_cluster(&_fs);
    else
    {
        // Find parent directory start cluster
        if (!_open_directory(file->path, &file->parentcluster))
            return NULL;
        
    }

    // Using dir cluster address search for filename
    if (fatfs_get_file_entry(&_fs, file->parentcluster, file->filename,&sfEntry))
        // Make sure entry is file not dir!
        if (fatfs_entry_is_file(&sfEntry))
        {
            // Initialise file details
            memmove_akel(file->shortfilename, sfEntry.Name, FAT_SFN_SIZE_FULL);
            file->filelength = FAT_HTONL(sfEntry.FileSize);
            file->startcluster = ((FAT_HTONS((uint32)sfEntry.FstClusHI))<<16) + FAT_HTONS(sfEntry.FstClusLO);
            file->f_pos = 0;
            return file;
        }

   
    return NULL;
}

int fat_fread(unsigned char * buffer, int size, int length, void *f )
{
 
  uint32 Cluster = 0;
  uint32 lba;
  int  first = 1 ;
  uint32  clustercount = 0, skeep = 0 ;

  unsigned char *block  = (unsigned char *) FS_BUFFER ;

  int nbyte = size * length;
  int bytesRead = 0; 

  FILE *file = (FILE *)f;
 
   
 
    if (buffer==NULL || file==NULL)
        return -1;
 
   // Nothing to be done
    if (!nbyte )
        return 0;
 
  // Limit to file size
    if( (file->f_pos + nbyte) > file->filelength )
        nbyte = file->filelength - file->f_pos;

  uint32 cluster_to_read  = nbyte / _fs.cluster_size ;
  


  uint32 offset = file->f_pos % _fs.cluster_size ;
if(file->f_pos < _fs.cluster_size )
    offset = file->f_pos ;
  int plus = _fs.cluster_size - offset ;

 uint32 remainder = nbyte % _fs.cluster_size ;
      
 


  Cluster = file->startcluster;

        // Advance until we are at the correct location
    while ((clustercount++ < (file->f_pos / _fs.cluster_size) ))
         Cluster = fatfs_find_next_cluster(&_fs,Cluster);
        
   

  
  while( Cluster!= 0x0ffffff){
 
  lba = fatfs_lba_of_cluster(&_fs, Cluster) ;
       
       
    
       if ( (bytesRead / _fs.cluster_size) == cluster_to_read  ){
          if(remainder){

              if (first && file->f_pos){

        media_read_ak(lba, block , _fs.cluster_size);
               
                memmove_akel (buffer, block + offset, remainder); 
                buffer += remainder ;
                bytesRead += remainder ; 
                first = 0;
                } else {
          
                 media_read_ak(lba, buffer, remainder );
                 bytesRead += remainder ;
                }

          }
          file->f_pos += bytesRead ;
          return bytesRead ;
         }

       
             if (first && file->f_pos){

        media_read_ak(lba, block , _fs.cluster_size);
               
                memmove_akel (buffer, block + offset, plus); 
                buffer += plus ;
                bytesRead += plus ;
                first = 0;
           }else{
                media_read_ak(lba,buffer, _fs.cluster_size);
                buffer += _fs.cluster_size ;
                bytesRead += _fs.cluster_size  ;
               }
     
      Cluster = fatfs_find_next_cluster(&_fs,Cluster);
 
   }
      file->f_pos += bytesRead ;
      return bytesRead ;
}


void fat_fclose( void *f )
{

        FILE *file = (FILE *)f;
        file->f_pos = 0;
        file->filelength = 0;
        file->startcluster = 0;
        file->parentcluster = 0;
     
}


