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

#ifndef __FILESTRING_H__
#define __FILESTRING_H__

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
int fatfs_total_path_levels(char *path);
int fatfs_get_substring(char *Path, int levelreq, char *output, int max_len);
int fatfs_split_path(char *FullPath, char *Path, int max_path, char *FileName, int max_filename);
int fatfs_compare_names(char* strA, char* strB);
int fatfs_string_ends_with_slash(char *path);
int fatfs_get_sfn_display_name(char* out, char* in);
int fatfs_get_extension(char* filename, char* out, int maxlen);
int fatfs_create_path_string(char* path, char *filename, char* out, int maxlen);

#ifndef NULL
    #define NULL 0
#endif

#endif
