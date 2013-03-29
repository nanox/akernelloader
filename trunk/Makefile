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


#See man make
LD_FILE=ak_bootstrap.ld
#See man gcc
CC=gcc
#See man ld
LD=ld
#See man objcopy
OBJCOPY=objcopy
CFLAGS=-c -g -Os -march=i686 
OBJ_FLAGS= -R .symtab -R .shstrtab -R .pdr -R .comment -R .note -R .hash -S -O binary
LDFLAGS=


all:bootstrap akel  smake

bootstrap: ak_bootstrap.S
	@echo "akernelloader compile the bootstrap program  .. "
	@ $(CC) $(CFLAGS) ak_bootstrap.S
	@ $(LD)  ak_bootstrap.o -o bootstrap.elf -T $(LD_FILE)
	@ $(OBJCOPY) $(OBJ_FLAGS) bootstrap.elf $@
	@rm -f *.o 


akel:ak_boot-inst.c
	@echo "akernelloader compile the akel program .. "
	@ $(CC) ak_boot-inst.c -o $@
	
smake:
	@cd ./akernelloader && $(MAKE)   #Sub make akernelloader
	


	
.PHONY: clean

clean:

	@cd ./akernelloader && $(MAKE) clean
	@echo "DIR akernelloader is clean .. "
	@rm -f *.o *.elf *.bin *.BIN *~ 
	@rm bootstrap  akel 
	
	

    
