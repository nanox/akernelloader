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

#ifndef _STDIO_AK_H_
#define _STDIO_AK_H_

#include "stdarg_ak.h"


typedef void (*fn_ptr_t)(unsigned c);

int do_printf(const char *fmt, va_list args, fn_ptr_t fn);
void printf_ak(const char *fmt, ...);
void putchar_ak(unsigned c);

#endif