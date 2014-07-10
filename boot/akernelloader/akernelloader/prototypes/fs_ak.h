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

#ifndef _FS_AK_H_
#define _FS_AK_H_ 

#include <types_ak.h>
#include <ext2/ext2fs_ak.h>

#ifndef SEEK_CUR
    #define SEEK_CUR    1
#endif

#ifndef SEEK_END
    #define SEEK_END    2
#endif

#ifndef SEEK_SET
    #define SEEK_SET    0
#endif

#ifndef EOF
    #define EOF         (-1)
#endif

#define FATFS_MAX_LONG_FILENAME         260

typedef struct FILE
{
    /* FAT */
    uint32                  parentcluster;
    uint32                  startcluster;
    uint32                  filelength;
    char                    path[FATFS_MAX_LONG_FILENAME];
    char                    filename[FATFS_MAX_LONG_FILENAME];
    uint8                   shortfilename[11];
    // File fopen flags
    uint8                   flags;
#define FILE_READ           (1 << 0)
#define FILE_WRITE          (1 << 1)
#define FILE_APPEND         (1 << 2)
#define FILE_BINARY         (1 << 3)
#define FILE_ERASE          (1 << 4)
#define FILE_CREATE         (1 << 5)

     __u32 f_pos;
      /*EXT2 ... 3*/

 struct ext2_inode *inode ;
 


} __attribute__ ((packed)) FILE;

struct FILE _file;


struct fs_op
      {
     struct FILE* (*open) (const char *path);
     int   (*read)  (unsigned char * buffer, int size, int length, void *f );
     void  (*close) (void *f );
      } fs_op_ak;

// Initialize fs_op_ak
int init_file_op(int fs_type);
struct FILE* fopen_ak(const char *pathname);
int fread_ak(unsigned char * buffer, int size, int length, void *f );
void fclose_ak( void *f );
int fseek_ak( void *f, long offset, int origin );

#endif
