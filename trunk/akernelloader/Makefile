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
 
 
# See man make
# The load boot loader
LD_FILE=akernelloader.ld
#See man gcc
CC=gcc
# See man ld
LD=ld
# See man objcopy
OBJCOPY=objcopy
CFLAGS= -Wall -nostdlib -nostdinc -Iprototypes 

OBJ_FLAGS=-R .pdr -R .comment -R .note -R .hash -S -O binary
LDFLAGS=-nostdlib -Wl -S 
path= ../

EXEC= akernelloader.elf

OBJECTS = akernelloader.o gdt_ak.o abios_ak.o bios_ak.o start_kernel_ak.o	\
          disk_ak.o printf_ak.o string_ak.o elf_ak.o readconf.o  main.o 	\
          fat_ak/fat_access.o fat_ak/fat_fs.o			\
          fat_ak/fat_misc.o fat_ak/fat_string.o fat_ak/fat_table.o  \
          ext2_ak/ext2fs_ak.o fs_op_ak.o

all:akernelloader.bin smake move

akernelloader.bin: $(OBJECTS) 
	@echo "akernelloader compile the program akernelloader.bin ... "
	@ $(LD) $(OBJECTS)  -o $(EXEC) -T $(LD_FILE)  
	@ $(OBJCOPY) $(OBJ_FLAGS) $(EXEC) $@
	

# Create objects from assembler (.S) and (.c) source code
%.o: %.S %.c 
	$(CC) -c $<  $(CFLAGS) -o $@

smake:
	@cd ./fat_ak && $(MAKE)   #Compile the fat directory
	@cd ./ext2_ak && $(MAKE)  #Compile the ext2 directory
move:
	@mv ./akernelloader.bin $(path)

.PHONY: clean

clean:
	@cd ./fat_ak && $(MAKE) clean
	@cd ./ext2_ak && $(MAKE) clean
	@rm -f *.elf *.o  *.bin *.BIN *~ 
	@echo "DIR akernelloader is clean .. "



