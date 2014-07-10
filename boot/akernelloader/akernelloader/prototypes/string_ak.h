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
#ifndef _STRING_AK_H_
#define _STRING_AK_H_
 


int   strlen_akel  (const char *string);
char *strcpy_akel  (char *dest, const char *src);
void *memmove_akel (void *to, const void *from, int length);
void *memset_akel  (void *s_addr, int c, int length);
int   strcmp_akel  (const char *s1, const char *s2);
int strncat_akel (char *s1, const char *s2, int n);
int isspace_akel (int c);
int strend_null(char *str);

/* Special graphics characters for IBM displays. */
#define DISP_UL		218
#define DISP_UR		191
#define DISP_LL		192
#define DISP_LR		217
#define DISP_HORIZ	196
#define DISP_VERT	179
#define DISP_LEFT	0x1b
#define DISP_RIGHT	0x1a
#define DISP_UP		0x18
#define DISP_DOWN	0x19


/* These are used to represent the various color states we use */
typedef enum
{
  /* represents the user defined colors for normal text */
  COLOR_STATE_NORMAL,
  /* represents the user defined colors for highlighted text */
  COLOR_STATE_HIGHLIGHT
} color_state;

/* see putchar_ak */
extern int str_current_color;



#endif /* End string_ak.h */
