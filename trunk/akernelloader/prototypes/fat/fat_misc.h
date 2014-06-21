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

#ifndef __FAT_MISC_H__
#define __FAT_MISC_H__

#include "fat_defs.h"
#include "fat_opts.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define MAX_LONGFILENAME_ENTRIES    20
#define MAX_LFN_ENTRY_LENGTH        13



//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------
struct lfn_cache
{
#if FATFS_INC_LFN_SUPPORT
    // Long File Name Structure (max 260 LFN length)
    uint8 String[MAX_LONGFILENAME_ENTRIES][MAX_LFN_ENTRY_LENGTH];
    uint8 Null;
#endif
    uint8 no_of_strings;
};

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void    fatfs_lfn_cache_init(struct lfn_cache *lfn, int wipeTable);
void    fatfs_lfn_cache_entry(struct lfn_cache *lfn, uint8 *entryBuffer);
char*   fatfs_lfn_cache_get(struct lfn_cache *lfn);
int     fatfs_entry_lfn_text(struct fat_dir_entry *entry);
int     fatfs_entry_lfn_invalid(struct fat_dir_entry *entry);
int     fatfs_entry_lfn_exists(struct lfn_cache *lfn, struct fat_dir_entry *entry);
int     fatfs_entry_sfn_only(struct fat_dir_entry *entry);
int     fatfs_entry_is_dir(struct fat_dir_entry *entry);
int     fatfs_entry_is_file(struct fat_dir_entry *entry);
int     fatfs_lfn_entries_required(char *filename);
void    fatfs_filename_to_lfn(char *filename, uint8 *buffer, int entry, uint8 sfnChk);


#endif
