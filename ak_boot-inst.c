/* Copyright (C) 2007 akernelloader TEAM
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "prototypes_ak/ak_version.h"

#define START_HERE(c,v) main( c, v)
#define RUTINE_OCT 440 /* Code area */

#define AKERNELLOADER_SZ  30000

int error_w( ssize_t error, char* name ){

/** Write return -1 if error */
if( error == -1 )
  {
    printf(" Error : Can not write to file < %s >\n", name );
     exit(0);
    }
     else  printf(" [OK]  Now writes the file %s ..\n",name);

}

int error_r( ssize_t error, char* name ){

/** Read return -1 if error */
if( error == -1 )
  {
    printf(" Error : Can not read the file < %s >\n", name);
     exit(0);
     }
     else  printf(" [OK]  Read the file %s ..\n", name);

}
/* Size of bootstrap */
int b_strap_sz( char* filename){
struct stat info ;
           /* File status */
        if (stat(filename, &info) == -1){
             printf(" Error : Can not find the size\n");
              exit(0);
             }
             
            /* File size in bytes */
        if( info.st_size > 446  ) { 
            printf(" Error : bootstrap has a size(%i) too big\n", 
                                               (int) info.st_size );
             exit(0);
            }
         else   printf(" %s [Size : %i B]\n", 
                                      filename,(int) info.st_size );
 
    
}

int compare_str(char *tab_arg , char *string ){

      int size = strlen(string);
      if ( strncmp( tab_arg , string , size ) == 0 
      && !  strcmp( tab_arg , string ))
       { return 1 ; } else { return 0 ;}
}

int ak_usage(){
                printf("AKeL HELP: \n"
                " USE : wboot <options> <boot_sector> <device> \n"
                " Options :\n"
                " -bs   Write the bootstrap in the first sector \n"
                " -ak   Write akernelloader \n"
                " -v    Print version number\n"
                " -h    Print this help\n");
                
                return 0 ;
}

int START_HERE(int argc,char *argv[])
{
/* Byte Order 
        This code is just for little-endian :(          */
         int word = 1; 
     char *p =(char*) & word ;
        if(p[0] == 1){  
                printf("Is Little Endian\n");
        }       
        else {
                printf("Is Big Endian STOP ...\n");
     exit(0); 
    }

     if(argc <= 1 ){
          ak_usage();
          return 0;
          }
        
        /* Bootstrap */
    if(compare_str(argv[1] , "-bs" )){  
    /*Is big ?.. > 512*/
    b_strap_sz(argv[2]);
    
    printf(" %s  ->  %s  \n",argv[2],argv[3]);
    
    char code_mbr[RUTINE_OCT];
    int file_d = open (argv[2], O_RDONLY );     /* Return the file descriptor */
    /* Read 440 B and save in code_mbr */
    error_r( read(file_d, code_mbr, RUTINE_OCT ) , argv[2] );
    close(file_d);
   
    char buff_mag[2]= { 0x55, 0xaa } ; 
    int fd = open (argv[3], O_WRONLY );
    lseek (fd,0 , SEEK_SET );
    error_w( write(fd, code_mbr, RUTINE_OCT ),argv[3] );
    /* Write magic */
    lseek (fd  ,510, SEEK_SET );
    error_w( write(fd,  buff_mag, 2 ), argv[3] );
    
    close(file_d);
    return 0;
    }


      
      /* Write BOOT_LOADER */
      
    if( compare_str(argv[1] , "-ak" )){
    
    
    
    printf(" %s  ->  %s  \n",argv[2],argv[3]);
    char buffer_akernelloader[AKERNELLOADER_SZ ]; /* Buffer */
    
    int fd_akernelloader = open (argv[2], O_RDONLY );       /* Return the file descriptor */
    /* Read size( AKERNELLOADER_SZ  ) and save in buffer */
    error_r( read(fd_akernelloader, buffer_akernelloader, AKERNELLOADER_SZ  ) , argv[2] );
    close(fd_akernelloader);
    
    int fd_transfer = open (argv[3], O_WRONLY );
    lseek (fd_transfer  ,512, SEEK_SET );
    error_w( write(fd_transfer, buffer_akernelloader,AKERNELLOADER_SZ  ), argv[3] );
   
    close(fd_transfer);
    return 0;
    }
    
    
    if( compare_str(argv[1] , "-v" )){
        puts("Akernelloader version:"_ak_version);
        return 0;
    }
    
   
     ak_usage(); /* help */
      
    
}    
