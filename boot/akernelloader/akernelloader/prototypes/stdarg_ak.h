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

#ifndef _STDARG_AK_H_
#define _STDARG_AK_H_

typedef unsigned char *va_list;

/* width of stack == width of int */
#define	STACKITEM	int

/* round up width of objects pushed on stack. The expression before the
& ensures that we get 0 for objects of size 0. */
#define	VA_SIZE(TYPE)					\
	((sizeof(TYPE) + sizeof(STACKITEM) - 1)	\
		& ~(sizeof(STACKITEM) - 1))

/* &(LASTARG) points to the LEFTMOST argument of the function call
(before the ...) */
#define	va_start(AP, LASTARG)	\
	(AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

#define va_end(AP)	/* nothing */

#define va_arg(AP, TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

#endif
