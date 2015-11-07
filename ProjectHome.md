## Description ##

AKernelLoader (AKeL) is a project open source without much ambition,to develop a bootloader single or multi-boot, everything depends on the participation of developers. I use the GPL because it more independence to developers.Can be an opportunity for those who are doing their first operating system kernel have a simple bootloader to start. All participation is welcome, if you have any comments or suggestions on the current code, please send email to akaloaderadmin@gmail.com .You can see my site <a href='http://akernelloader.freehostia.com'>here</a>.



## Boot Loader ##
Briefly, a bootloader is the first software program that runs when a computer starts. It is responsible for loading and transferring control to the operating system kernel.
The kernel, in turn, initializes the rest of the operating system.

## Details ##

<b>Language:</b>  AKeL coded with C language and GNU assembly.<br>

<b>Tools:</b>  Akernelloader includes a small program to write in the mbr.<br>
<br>
<b>Supported devices :</b>
<ul><li>Disk<br>
</li><li>USB Flash Drive</li></ul>

<b>Supported file system :</b>
<ul><li>FAT32<br>
</li><li>ext2<br>
</li><li>ext3</li></ul>

<b>Supported kernel :</b>
<ul><li>ELF ( Executable and Linkable Format ).<br>
</li><li>Binary or Raw.<br>
</li><li>With standard multiboot header.<br>
</li><li>Linux.</li></ul>

<h2>Install</h2>
To install and use you should read the file "install" included in the source code.<br>
<br>
<h2>Emulator test results:</h2>
Before testing on a real computer we tested on an emulator pc.<br>
<b>BOCHS:</b>
<ul><li>stability: GOOD<br>
</li><li>functionality: OK</li></ul>

<b>QEMU:</b>
<ul><li>stability: GOOD<br>
</li><li>functionality: OK</li></ul>

You can see <a href='http://code.google.com/p/akernelloader/source/browse/trunk'>here</a> the source code of the bootloader Akernelloader.<br>
<br><br>

Very good for testing your OS on usb ... Look  <a href='http://wiki.osdev.org/Expanded_Main_Page'>OSDev.org</a><br><br>

<img src='http://akernelloader.googlecode.com/svn/branches/akernelloader.jpg' />
