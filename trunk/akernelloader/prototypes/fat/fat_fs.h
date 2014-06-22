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

#ifndef __FAT_FS_H__
#define __FAT_FS_H__

#include "fat_access.h"
#include <fs_ak.h>


#define FS_BUFFER 0x68000


void fat_init(void);
struct FILE* fat_fopen(const char *path);
int fat_fread(unsigned char* buffer, int size, int length, void* f );
void fat_fclose( void *f );

#endif
