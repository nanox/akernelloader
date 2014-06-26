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

    /* see putchar_ak */
int str_current_color =  0x07 ;
/*
List of BIOS color attributes

 0 	0000 	Black 	 
 1 	0001 	Blue 	
 2 	0010 	Green 	
 3 	0011 	Cyan 	
 4 	0100 	Red 	
 5 	0101 	Magenta 	
 6 	0110 	Brown 	
 7 	0111 	Light Gray 	
 8 	1000 	Dark Gray 	
 9 	1001 	Light Blue 	
 A 	1010 	Light Green 	
 B 	1011 	Light Cyan 	
 C 	1100 	Light Red 	
 D 	1101 	Light Magenta 	
 E 	1110 	Yellow 	
 F 	1111 	White 	
*/
void set_str_color(color_state state){

 switch (state) {
    case COLOR_STATE_NORMAL:
      str_current_color =  0x07 ;
      break;
    case COLOR_STATE_HIGHLIGHT:
      str_current_color = 0x70 ;
      break;
    default:
      str_current_color = 0x07 ;
      break;
  }
      

}

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


int
tolower (int c)
{
  if (c >= 'A' && c <= 'Z')
    return (c + ('a' - 'A'));

  return c;
}

int
ak_parse_maxint (char **str_ptr, int *myint_ptr)
{
  char *ptr = *str_ptr;
  int myint = 0;
  int mult = 10, found = 0;
  int errnum;
  /*
   *  Is this a hex number?
   */
  if (*ptr == '0' && tolower (*(ptr + 1)) == 'x')
    {
      ptr += 2;
      mult = 16;
    }

  while (1)
    {
      /* A bit tricky. This below makes use of the equivalence:
	 (A >= B && A <= C) <=> ((A - B) <= (C - B))
	 when C > B and A is unsigned.  */
      unsigned int digit;

      digit = tolower (*ptr) - '0';
      if (digit > 9)
	{
	  digit -= 'a' - '0';
	  if (mult == 10 || digit > 5)
	    break;
	  digit += 10;
	}

      found = 1;
      if (myint > (( 0x7FFFFFFF - digit) / mult))
	{
	  errnum = -1 ;
	  return 0;
	}
      myint = (myint * mult) + digit;
      ptr++;
    }

  if (!found)
    {
      errnum = -1 ;
      return 0;
    }

  *str_ptr = ptr;
  *myint_ptr = myint;

  return 1;
}

