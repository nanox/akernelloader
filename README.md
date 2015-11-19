Description
===========
AKernelLoader (AKeL) is a project open source without much ambition,to develop a bootloader single or multi-boot, everything depends on the participation of developers. I use the GPL because it more independence to developers.Can be an opportunity for those who are doing their first operating system kernel have a simple bootloader to start. All participation is welcome, if you have any comments or suggestions on the current code, please send email to akaloaderadmin@gmail.com .You can see my site here.

Boot Loader
===========
Briefly, a bootloader is the first software program that runs when a computer starts. It is responsible for loading and transferring control to the operating system kernel. The kernel, in turn, initializes the rest of the operating system.

Details
=======

Language: 
---------
AKeL coded with C language and GNU assembly.

Tools: 
------
Akernelloader includes a small program to write in the mbr. 

Supported devices:
------------------
* Disk
* USB Flash Drive

Supported file system: 
----------------------
* FAT32
* ext2
* ext3

Supported kernel:
-----------------
* ELF ( Executable and Linkable Format ).
* Binary or Raw.
* With standard multiboot header.
* Linux.

Install
=======
To install and use you should read the file "install" included in the source code.

Emulator test results:
=====================
Before testing on a real computer we tested on an emulator pc.

BOCHS:
-----
* stability: GOOD
* functionality: OK

QEMU:
-----
* stability: GOOD
* functionality: OK

You can see here the source code of the bootloader Akernelloader. 

Very good for testing your OS on usb ... Look OSDev.org

![AKeL](http://akernelloader.freehostia.com/bootloader.jpg)
