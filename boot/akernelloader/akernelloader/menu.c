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
#include <stdio_ak.h>
#include <config_ak.h>
#include <multiboot_ak.h>
#include <readconf.h>
#define __DEF_MENU__ 1

#include <menu.h>

void
init_menu_page (void)
{
  cls_ak();

  printf_ak ("\n  Akernelloader version 0.0.1\n\n");
}


char * get_entry (char *list, int num, int nested)
{
  int i;


  for (i = 0; i < num; i++)
    {
      do
	{
	  while (*(list++));
	}
      while (nested && *(list++) );
    }

  return list;
}


void
print_entry_ak(int y, int highlight, char *entry)
{
  int x;

  
  if (highlight)
    set_str_color(COLOR_STATE_HIGHLIGHT);

  gotoxy_ak (2, y);
  putchar_ak (' ');
  for (x = 3; x < 75; x++)
    {
      if (*entry && x <= 72)
	{
	  if (x == 72)
	    putchar_ak (DISP_RIGHT);
	  else
	    putchar_ak (*entry++);
	}
      else{
        set_str_color(COLOR_STATE_NORMAL);
	putchar_ak (' ');
        }
    }
  gotoxy_ak(74, y);

  set_str_color(COLOR_STATE_NORMAL);
}



/* Print entries in the menu box.  */
void
print_entries_ak (int y, int size, int first, int entryno, char *menu_entries)
{
  int i;
  
  gotoxy_ak (77, y + 1);
 /** Sure ahahha */
  if (first)
    putchar_ak (DISP_UP);
  else
    putchar_ak (' ');

 char* entries = get_entry (menu_entries, first, 0);

  for (i = 0; i < size; i++)
    {
      print_entry_ak (y + i + 1, entryno == i, entries);

      while (*entries)
	entries++;

      if (*(entries - 1))
	entries++;
    }

  gotoxy_ak(77, y + size);

  if (*entries)
    putchar_ak (DISP_DOWN);
  else
    putchar_ak (' ');

  gotoxy_ak(74, y + entryno + 1);
}

int run_menu_akernelloder(){

   

   int c;
   int entryno = 0,first_entry = 0;
   init_menu_page ();
   setcursor_ak (0);

   print_entries_ak(3, 12, 0, 0, menu_entries);
 gotoxy_ak(3,20);
 printf_ak ("Use the %c and %c keys to select which entry is highlighted\n" 
            , DISP_UP, DISP_DOWN);
 gotoxy_ak(3,21);
 printf_ak ("and press enter to boot the selected OS\n");
 

 while(1){

c = ASCII_CHAR (getkey_ak ());

if (c == 16 || c == '^'){

if (entryno > 0)
		    {
		      print_entry_ak (4 + entryno, 0,
				   get_entry (menu_entries,
					      first_entry + entryno,
					      0));
		      entryno--;
		      print_entry_ak (4 + entryno, 1,
				   get_entry (menu_entries,
					      first_entry + entryno,
					      0));
		    }
		  else if (first_entry > 0)
		    {
		      first_entry--;
		      print_entries_ak (3, 12, first_entry, entryno,
				     menu_entries);
		    }

}


if (c == 14 || c == 'v'){

	 
		  if (entryno < 11 && (entryno + 1) < num_menu_entries)
		    {
		      print_entry_ak (4 + entryno, 0,
				   get_entry (menu_entries,
					      first_entry + entryno,
					      0));
		      entryno++;
		      print_entry_ak ( 4 + entryno, 1,
				   get_entry (menu_entries,
					      first_entry + entryno,
					      0));
		  }
		else if (num_menu_entries > 12 + first_entry)
		  {
		    first_entry++;
		    print_entries_ak (3, 12, first_entry, entryno, menu_entries);
		  }

}
     

      if ((c == '\n') || (c == '\r') || (c == 6) ){

      cls_ak();
      gotoxy_ak(0,0);

      char * ptr;
      char *arg;
      int module_count = 0;
      char *old_entry;
      char *cur_entry = get_entry(config_entries,first_entry + entryno,1);
      char *heap;	
      printf_ak ("Boot configuration:\n");

      do{

      *(cur_entry--);
      old_entry = cur_entry;
      while (*cur_entry++);

      memmove_akel (heap, old_entry, (int) cur_entry - (int) old_entry );

      set_str_color(COLOR_STATE_NORMAL);
      
      printf_ak ("%s  \n", heap );

      ptr = find_command (heap);
      arg = find_arg (heap);

     if(ptr){
      
        if (!strcmp_akel (ptr,"kernel")){
        strend_null(arg);
        strcpy_akel(kernel_path,arg );
       
       }
        if (!strcmp_akel (ptr,"initrd")){
        strend_null(arg);
        strcpy_akel (initrd_path, arg );  
      }

        if (!strcmp_akel (ptr,"module")){
        strend_null(arg);
        strcpy_akel (module_path[module_count], arg );  
        module_count++;
     }

       if (!strcmp_akel (ptr,"root")){
       strend_null(arg);
       strcpy_akel (device_str, arg ); 

    }
 }
       
       }while(*(cur_entry++));

        break ;

    }
}
   
   return 0;     
}


