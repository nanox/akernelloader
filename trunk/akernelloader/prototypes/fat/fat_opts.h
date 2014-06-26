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


#ifndef __FAT_OPTS_H__
#define __FAT_OPTS_H__

#ifdef FATFS_USE_CUSTOM_OPTS_FILE
    #include "fat_custom.h"
#endif

//-------------------------------------------------------------
// Configuration
//-------------------------------------------------------------

// Is the processor little endian (1) or big endian (0)
#ifndef FATFS_IS_LITTLE_ENDIAN
    #define FATFS_IS_LITTLE_ENDIAN          1
#endif


// Max open files (reduce to lower memory requirements)
#ifndef FATFS_MAX_OPEN_FILES
    #define FATFS_MAX_OPEN_FILES            2
#endif

// Number of sectors per FAT_BUFFER (min 1)
#ifndef FAT_BUFFER_SECTORS
    #define FAT_BUFFER_SECTORS              1
#endif

// Max FAT sectors to buffer (min 1)
// (mem used is FAT_BUFFERS * FAT_BUFFER_SECTORS * FAT_SECTOR_SIZE)
#ifndef FAT_BUFFERS
    #define FAT_BUFFERS                     1
#endif

// Size of cluster chain cache (can be undefined)
// Mem used = FAT_CLUSTER_CACHE_ENTRIES * 4 * 2
// Improves access speed considerably
//#define FAT_CLUSTER_CACHE_ENTRIES         128

// Include support for writing files (1 / 0)? 
#ifndef FATFS_INC_WRITE_SUPPORT
    #define FATFS_INC_WRITE_SUPPORT         1
#endif

// Support long filenames (1 / 0)? 
// (if not (0) only 8.3 format is supported)
#ifndef FATFS_INC_LFN_SUPPORT
    #define FATFS_INC_LFN_SUPPORT           1    
#endif

// Support directory listing (1 / 0)?
#ifndef FATFS_DIR_LIST_SUPPORT
    #define FATFS_DIR_LIST_SUPPORT          1
#endif

// Include support for formatting disks (1 / 0)?
#ifndef FATFS_INC_FORMAT_SUPPORT
    #define FATFS_INC_FORMAT_SUPPORT        1
#endif

// Sector size used
#define FAT_SECTOR_SIZE                     512

#endif
