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



#include <fs_ak.h>
#include <stdio_ak.h>
#include <string_ak.h>
#include <config_ak.h>
#include <multiboot_ak.h>
#include <menu.h>
#include <readconf.h>

 int offset;
 int left;


char takeone(FILE * f);


char takeone(FILE * f) {
unsigned char textbuf[4096];

 if ((left - offset)<=0) {
 
  if (f->f_pos >= f->filelength){
     printf_ak("End of config ..  \n" );
      return (-1);
  }

  left = fread_ak(textbuf,1,4096,f );
  offset = 0;
  if (left == 0) return -1;
 }

 return textbuf[offset++];
}
int getaline(FILE * f , char * cmdline, int size) {
 
 int pos = 0, literal = 0, comment = 0;
 char c;  /* since we're loading it a byte at a time! */
  
 c = takeone(f);
 if (c < 0) return 0;
 while (c >=0 && size > 1)
    {
          c = takeone(f);

      /* Skip all carriage returns.  */
      if (c == '\r')
	continue;

      /* Replace tabs with spaces.  */
      if (c == '\t')
	c = ' ';

      /* The previous is a backslash, then...  */
      if (literal)
	{
	  /* If it is a newline, replace it with a space and continue.  */
	  if (c == '\n')
	    {
	      c = ' ';
	      
	      /* Go back to overwrite a backslash.  */
	      if (pos > 0)
		pos--;
	    }
	    
	  literal = 0;
	}
	  
      /* translate characters first! */
      if (c == '\\' && ! literal)
	literal = 1;

      if (comment)
	{
	  if (c == '\n')
	    comment = 0;
	}
      else if (! pos)
	{
	  if (c == '#')
	    comment = 1;
	  else if ((c != ' ') && (c != '\n'))
	    cmdline[pos++] = c;
	}
      else
	{
	  if (c == '\n')
	    break;

	    cmdline[pos++] = c;
            size--;
	}
    }

  cmdline[pos] = 0;

 return 1;
}

char *
find_arg ( char *cmdline)
{
/* Skip until we hit whitespace */
while (*cmdline == ' ' || *cmdline== '\t')
    cmdline++;

  while (*cmdline != '/' && *cmdline != '\t' && *cmdline != ' ' )
    cmdline ++;

  /* Skip whitespace, and maybe equal signs. */
  while ( *cmdline == ' ' || *cmdline == '\t' ||*cmdline == '=')
    cmdline ++;

  return cmdline;
}

char *find_command (char *command)
{
  char *ptr;

   while (*command == ' ' || *command== '\t')
          command ++;
  
  /* Find the first space and terminate the command name.  */
  ptr = command;
  while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '=')
    ptr ++;

*ptr = 0;
  
     if(!strcmp_akel (command,"kernel")){
         *ptr = ' ';  
         return "kernel";
       }
       else if(!strcmp_akel (command,"initrd")){
          *ptr = ' ';  
          return "initrd";
      }
       else if(!strcmp_akel (command,"module")){ 
          *ptr = ' '; 
          return "module";
      }
     else if(!strcmp_akel (command,"root")){ 
          *ptr = ' '; 
	  return "root";
          }
    else if(!strcmp_akel (command,"title")) {
          *ptr = ' '; 
	  return "title";
          }
      *ptr = ' '; 
  return NULL;

}



int read_fconfig(const char* conf_path){

  char line[254];

 char * ptr;
 char *arg;
 int menu_len = 0 , config_len = 0 , state = 0;

 FILE *fd;
 
 fd = fopen_ak(conf_path);
 if(fd == NULL){
      printf_ak("Not open the configuration file .. \n");
      getkey_ak();
 }

 while (getaline(fd ,line,254)){ 
   
  ptr = find_command (line);
 

      
  if (ptr == "title"){
      if(state > 0 )
      config_entries[config_len++] = 0;
      num_menu_entries++;
      arg = find_arg (line);
       while ((menu_entries[menu_len++] = *(arg++)) != 0);
             
     }

    char *ptr = line;

   /* Copy config file data to config area.  */
    while ((config_entries[config_len++] = *ptr++) != 0);
     
    state++;
    
 }


return 0;

}


 



