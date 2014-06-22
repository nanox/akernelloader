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
#include <fs_ak.h>
#include <ext2/ext2fs_ak.h>
#include <fat/fat_fs.h>


int init_file_op(int fs_type){

if( fs_type == PARTITION_FAT32 || PARTITION_FAT32_LBA ){

        fat_init();

	fs_op_ak.open =  fat_fopen ;
	fs_op_ak.read =  fat_fread ;
        fs_op_ak.close =  fat_fclose ;

}

if( fs_type == PARTITION_EXT2 ){

	 ext2fs_init_ak();

	fs_op_ak.open =  ext2_fopen;
	fs_op_ak.read =  ext2_fread ;
        fs_op_ak.close =  ext2_fclose ;
}

 return 0;

}

struct FILE* fopen_ak(const char *pathname){

  return fs_op_ak.open(pathname);
}

int fread_ak(unsigned char * buffer, int size, int length, void *f ){
 
   return fs_op_ak.read(buffer ,size, length, f );
}

void fclose_ak( void *f ){

   return fs_op_ak.close(f);
}

int fseek_ak( void *f, long offset, int origin )
{
    FILE *file = (FILE *)f;
    int res = -1;


    if (!file)
        return -1;

    if (origin == SEEK_END && offset != 0)
        return -1;



    if (origin == SEEK_SET)
    {
        file->f_pos= (uint32)offset;

        if (file->f_pos > file->inode->i_size)
            file->f_pos = file->inode->i_size;

        res = 0;
    }
    else if (origin == SEEK_CUR)
    {
        // Positive shift
        if (offset >= 0)
        {
            file->f_pos += offset;

            if (file->f_pos > file->inode->i_size)
                file->f_pos = file->inode->i_size;
        }
        // Negative shift
        else
        {
            // Make shift positive
            offset = -offset;

            // Limit to negative shift to start of file
            if ((uint32)offset > file->f_pos)
                file->f_pos = 0;
            else
                file->f_pos-= offset;
        }

        res = 0;
    }
    else if (origin == SEEK_END)
    {
        file->f_pos = file->inode->i_size;
        res = 0;
    }
    else
        res = -1;

    return res;
}





 
