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

#include <ext2/ext2fs_ak.h>
#include <partition_ak.h>
#include <disk_ak.h>
#include <string_ak.h>
#include <stdio_ak.h>

struct fs_data _fs;

void
get_block (unsigned long bnum, unsigned char *block)
{
    
    
        uint64 to_seek = bnum;
        to_seek = (to_seek * BLOCKSIZE) / 512  ; 
        to_seek += partition.start;
         if(media_read_ak( to_seek , block, BLOCKSIZE )  == 0)
            printf_ak("EXT2 READ ERROR..\nlba:%i\nBlk number:%i\n",to_seek,bnum);
       
}

int 
get_inode (int inum, struct ext2_inode *inode)
{
    int group; 
    unsigned int bnum; 
    unsigned char block[BLOCKSIZE]; 
    struct ext2_inode *p; 
        struct fs_data *fs = &_fs;
    int n; 
    int inode_in_group, block_in_group, inode_in_block; 

    group = (inum - 1)  / fs->sb->s_inodes_per_group; 
    inode_in_group = (inum - 1) % fs->sb->s_inodes_per_group; 
    block_in_group = inode_in_group / fs->inodes_per_block;
    inode_in_block = inode_in_group % fs->inodes_per_block; 

    bnum = fs->groups[group].bg_inode_table + block_in_group;  

    get_block (bnum, block); 

    p = (struct ext2_inode*) 
        (block + (inode_in_block * fs->sb->s_inode_size)); 
        if (fs->sb->s_inode_size  <= sizeof (struct ext2_inode))
                n = fs->sb->s_inode_size;
        else
                n = sizeof (struct ext2_inode);

        memmove_akel( (void*) inode, (void *) p, n); 
    return (0); 
}

int
parse_pathname (const char path[], char names[MAX_DIRS][MAX_DIRNAME])
{
    int i, k; 
    int len; 
    char astr[MAX_DIRNAME]; 
    int n; 

    i = 0; 
    n = 0; 
    len = strlen_akel(path); 


    if (len <=0 )
        return -1; 

    if (path[0] != '/') 
        return -1;

    strcpy_akel (names[n], "/"); 

    n++; 
    i = 1; 
    while (i < len) {
        k = 0; 
        while (path[i] != '/') {
            astr[k] = path[i]; 
            k++; 
            i++; 
            if (i == len) 
                break; 
        }
        if (k == 0) 
            return -1; 

        astr[k] = '\0'; 
        strcpy_akel (names[n], astr); 
        n++;
        i++; 
    }


    return (n); 
}

int
get_data_block ( struct ext2_inode *inode, 
      int n, /* requested file data block */
      unsigned char *block) 
{

struct fs_data *fs = &_fs;

    unsigned char ind_block[4096]; 
    unsigned char dind_block[4096] ;
    unsigned char tind_block[4096] ;

    unsigned int size; /* size of file in blocks */
    unsigned int *p1, *p2, *p3;  

 int ClustByteShift = fs->sb->s_log_block_size + 10;

 int PtrsPerBlock1 = 1 << (ClustByteShift - 2);
 int PtrsPerBlock2 = 1 << ((ClustByteShift - 2) * 2);
 int PtrsPerBlock3 = 1 << ((ClustByteShift - 2) * 3);



if (inode->i_size == 0)
        size = 0; 
    else 
        size = 1 + ( (inode->i_size - 1) / BLOCKSIZE );

    if (size  == 0) 
        return -1; 

    if ( (n < 0)  || (n >=size)) 
         return -1; 

  /* direct blocks */
        if (n < EXT2_NDIR_BLOCKS) {
                 // direct blocks
          get_block (inode->i_block[n], block);
          return 0; 
       } 
    
        /* indirect blocks */
        n -= EXT2_NDIR_BLOCKS;
        if (n < PtrsPerBlock1) {
           get_block ( inode->i_block[EXT2_IND_BLOCK], ind_block);
               p1 = (unsigned int *) &ind_block;
               get_block (p1[n], block);
               return 0;
        }
       
        /* double indirect blocks */
        n -= PtrsPerBlock1;
        if (n < PtrsPerBlock2) {
             get_block (inode->i_block[EXT2_DIND_BLOCK], dind_block);
                 p2 = (unsigned int *) &dind_block;
           

               get_block ( p2[n / PtrsPerBlock1] , ind_block);
               p1 = (unsigned int *) &ind_block;
               get_block ( p1[n % PtrsPerBlock1] , block);
            
           return 0;
            
        }
       
             /* triple indirect block */
         n -= PtrsPerBlock2;
        if (n < PtrsPerBlock3) {

                 get_block (inode->i_block[EXT2_TIND_BLOCK], tind_block);
                 p3 = (unsigned int *) &tind_block;
           
                 get_block (p3[n/ PtrsPerBlock2], dind_block);
                 p2 = (unsigned int *) &dind_block;

           
             get_block (p2[n % PtrsPerBlock2], ind_block);
                 p1 = (unsigned int *) &ind_block;

            get_block (p1[n % PtrsPerBlock1], block);
    
            return 0;
        }
       
       
        /* File too big, can not handle */
        printf_ak("ext2 ERROR, file too big\n");
         
    return -1; 


}

int find_dir_entry(struct ext2_inode *dir_inode, char *name, 
           struct ext2_dir_entry_2  *diren)
{
        
    struct ext2_dir_entry_2  *dep; 
        int i, count; 
        char entry_name[MAX_DIRNAME];
        unsigned char *block = (unsigned char *) DATA_ADDR ;
    int logical; 
    

    logical  = 0; 
        get_data_block (dir_inode, logical, block);
        i = 0;
    count = 0; 
         
        while (1) {
        dep = (struct  ext2_dir_entry_2*) (block + i);

                memmove_akel(entry_name, dep->name, dep->name_len);
                entry_name[dep->name_len] = '\0';


                if (strcmp_akel(entry_name, name)  == 0) {
            memmove_akel( (void *) diren, (void *) dep, 
                 sizeof (struct   ext2_dir_entry_2));
                        return (0); 

        }
                i += dep->rec_len;
        count += dep->rec_len;

        if (count >= dir_inode->i_size) 
            break; 
        
                if (i >= BLOCKSIZE) {
            i = i % BLOCKSIZE; 
            logical++; 
            get_data_block (dir_inode, logical, block);
        }
        }
        return (-1);
}



int ext2fs_init_ak()
{
    struct fs_data *fs = &_fs;
    unsigned char block[4096] ;
    int i, j, gd_per_block; 
    int blk_count;
    struct ext2_group_desc *g; 

        fs->sb = SUPER_BLOCK_P ;

        uint64 sector = SUPER_BLOCK;
        sector = (sector * 4096) / 512  ; 

           // READ Super Block
         if(media_read_ak(sector + partition.start , block, 4096 )  == 0)
            printf_ak("EXT2 READ ERROR... \n"); 

    memmove_akel((void*)fs->sb, 
         (void*)(block + SUPER_BLOCK_OFFSET), 
         sizeof (struct ext2_super_block) );

        fs->group_count = 1 + 
        ((fs->sb->s_blocks_count - 1 ) / fs->sb->s_blocks_per_group); 
        fs->inodes_per_block = BLOCKSIZE / fs->sb->s_inode_size;
        fs->inodeblocks_per_group = fs->sb->s_inodes_per_group / fs->inodes_per_block;

     /* Ext2 filesystem divides a partition into block groups. 
        How many block group can there be in a filesystem/partition? 
        That depends on the block size and the partition size */

    fs->groups = GROUP_DESC_P ; 
        
        gd_per_block  =  BLOCKSIZE / sizeof (struct ext2_group_desc);
        blk_count = 1 + ((fs->group_count - 1) / gd_per_block); 
         
        for (i = 0; i < blk_count-1; ++i) {
              // Block Group Descriptor Table starts at block 1
                get_block ( GROUP0_GDT_BLOCK1 + i, block);
                g = (struct ext2_group_desc *) block;

                for (j = 0; j < gd_per_block; ++j) {
                        memmove_akel( (void *) &(fs->groups[i * gd_per_block + j]),
                                 (void *) &g[j],
                                 sizeof (struct ext2_group_desc));
                }
        }

    /* read last block of group descriptor table */
    get_block (GROUP0_GDT_BLOCK1 + i, block);
    g = (struct ext2_group_desc *) block;

                              
    for (j = 0; j < (fs->group_count % gd_per_block);  ++j) {
        memmove_akel( (void *) &(fs->groups[i * gd_per_block + j]),
             (void *) &g[j],
             sizeof (struct ext2_group_desc));
    }

    
    return 0; 
}





/* 
   pathname must be an absolute pathname
   find inode..
*/
int path_to_inode ( const char *path)
{
        struct ext2_inode *inode = INODE_P;

        struct ext2_dir_entry_2 dep;
    struct ext2_inode nod; 
        char names[MAX_DIRS][MAX_DIRNAME];
        int name_count = 0;
        int i;
    int ret; 
    
        name_count = parse_pathname (path, names);
        if (name_count == -1) {
               printf_ak ("parse error for pathname\n");
                return 0;
        }

    /* 
       create a directory entry for root dir
    */
    dep.file_type = EXT2_FT_DIR; 
    dep.inode = EXT2_ROOT_INO;  
    dep.name_len = 1; 
    dep.name[0] = '/'; 
    dep.rec_len = 1; 

        get_inode (dep.inode, &nod);

        
    i = 1; 
        while ((i < name_count) && (dep.file_type == EXT2_FT_DIR)  ) {
              
        ret = find_dir_entry (&nod, names[i], &dep);
     
        if (ret == -1) {
            printf_ak ("can not find dir entry %s\n", names[i]);
            return (-1); 
        }
        
        get_inode (dep.inode, &nod);
        i++; 
    }

    memmove_akel ( (void *)inode, (void*) &nod, sizeof (struct  ext2_inode));
    
    return 0; 
}

struct FILE* ext2_fopen(const char *pathname){

   
   // Allocate a new file handle
   FILE *file  = (struct FILE *) &_file;

   int ret;
           
        ret = path_to_inode (pathname);
                if (ret == -1) {
                    printf_ak("Path to inode error ...\n");    
                    return NULL;
                } 

   file->inode = (struct ext2_inode *)INODE_P ;
   file->f_pos = 0 ;
   file->filelength = file->inode->i_size;

          return file;
        

}

int ext2_fread(unsigned char * buffer, int size, int length, void *f )
{
       FILE *file = (FILE *)f;
       int bytesRead = 0; 
       int nbyte = size * length;
       unsigned char *block = (unsigned char *)DATA_ADDR;
       __u32 ret;
           

       int blk_to_read , remainder ; 
       int i = 0 ; 
    
    if (file->inode->i_size == 0) 
        return 0; 


  // Limit to file size
    if( (file->f_pos + nbyte) > file->inode->i_size )
         nbyte = file->inode->i_size - file->f_pos;

    blk_to_read = (nbyte + file->f_pos) / BLOCKSIZE;

    uint32 offset = file->f_pos % BLOCKSIZE ;
if(file->f_pos < BLOCKSIZE  )
    offset = file->f_pos ;
    unsigned int plus = BLOCKSIZE  - offset ; 
    
     remainder =  nbyte % BLOCKSIZE;
     remainder -= offset; 
 //first block
 int first  = 1;

// Utilize it to indicate the initial offset
i =  file->f_pos / BLOCKSIZE;
   while(i < blk_to_read) {

            
        ret = get_data_block ( file->inode, i, block); 
        if (ret == -1) {
            printf_ak ("can not get data block %i \n", i); 
            return 0; 
        }
             
       
                    if(first && file->f_pos){
                   get_data_block ( file->inode, i, block);
                   memmove_akel (buffer, block + offset, plus); 
                   buffer += plus;
                   bytesRead  += plus;
                   first = 0;

                 }else{
                   memmove_akel (buffer, block, BLOCKSIZE); 
                   buffer += BLOCKSIZE;
                   bytesRead  += BLOCKSIZE ;
                  }

               i++;   
              
    }


           if (first && file->f_pos ){
                  get_data_block ( file->inode, i, block);
                  memmove_akel (buffer, block + plus  ,remainder);             
                  buffer += remainder;
                  bytesRead  += remainder;
            }

        if(remainder){
           get_data_block ( file->inode, i, block);
           memmove_akel (buffer, block, remainder);
             bytesRead += remainder ;
                  
       }
         
    file->f_pos += bytesRead ;
    return bytesRead ;
}


void ext2_fclose( void *f )
{

        FILE *file = (FILE *)f;
        file->f_pos = 0;
        file->filelength = 0;
      
        
     
}
