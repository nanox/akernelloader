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


#include  <string_ak.h>

int   strlen_akel (const char *string){
 int length_ak = 0;

  while (*string++)
    length_ak++;

   return length_ak;
}

void *memmove_akel (void *to, const void *from, int length){
           
  char *dstp;
  const char *srcp;

  srcp = from;
  dstp = to;
  if (srcp < dstp)
    while (length-- != 0)
      dstp[length] = srcp[length];
  else
    while (length-- != 0)
      *dstp++ = *srcp++;

  return to;

}


char *strcpy_akel (char *dest, const char *src){

  memmove_akel (dest, src, strlen_akel(src) + 1);
  return dest;

}

void *memset_akel (void *s_addr, int c, int length){

 char *p_ak = s_addr;

      while (length -- > 0)
	*p_ak ++ = c;
   

  return s_addr ;


}

int strcmp_akel (const char *s1, const char *s2)
{
  while (*s1 || *s2)
    {
      if (*s1 < *s2)
	return -1;
      else if (*s1 > *s2)
	return 1;
      s1 ++;
      s2 ++;
    }

  return 0;
}

int isspace_akel (int c)
{
  switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      return 1;
    default:
      break;
    }

  return 0;
}

int strend_null(char *str)
{
  int ch;
  
  while (*str && ! isspace_akel (*str))
    str++;

  ch = *str;
  *str = 0;
  return ch;
}

