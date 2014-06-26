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
#ifndef __MENU_H__
#define __MENU_H__	1

#define MENU_BUF	0x78000
#define MENU_BUFLEN	0x6000
#define CONFIG_BUF	(MENU_BUF + MENU_BUFLEN)


#ifdef __DEF_MENU__
char * menu_entries = (char *) MENU_BUF;
char * config_entries = (char *) CONFIG_BUF;
#else
extern char * menu_entries ;
extern char * config_entries ;
#endif

int num_menu_entries ;
int run_menu_akernelloder();

#endif
