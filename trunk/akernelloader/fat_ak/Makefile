# Copyright (C) 2007 AKeL TEAM
#    akaloaderadmin@gmail.com

#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
   
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
#   USA. 

#See man gcc
CC=gcc
# See man ld
LD=ld

CFLAGS= -Wall -nostdlib -nostdinc -Iprototypes 

all:fat 

fat:  	

# Create objects from assembler (.S) and (.c) source code
%.o: %.c 
	$(CC) -c $<  $(CFLAGS) -o $@

.PHONY: clean

clean:
	@rm -f *.elf *.o  *.bin *.BIN *~ 
	@echo "DIR fat_ak is clean .. "
