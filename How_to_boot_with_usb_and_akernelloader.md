Xo#summary One-sentence summary of this page.

# How to boot from usb with akernelloader #
Akernelloader boot from usb with fat32 filesystem.<br> In the current state akernelloader lets you run a kernel on a disk or USB flash drive in a fat32 or ext2,ext3 file system.First you must develop your kernel and configure the file config_ak.h .<br>
If you do not have a kernel to test akernelloader you can use the kernel <a href='http://code.google.com/p/desiros-kernel/'>Desiros</a> is another of my projects that you can collaborate.<br>
<br>
<b>Virtual disk</b><br>
If you do not want to test the usb you can create a virtual disk. <br>If you are using Desiros create a virtual disk and format ext2 and copy the userland programs folder in your virtual disk.<br><br>

<b>Following are the commands to usb.</b>

<h2>Create a new partition</h2>
<br>

fdisk /dev/my_usb <br>

1. Setup geometry:<br>
x - extra functionality<br>
s 63<br>
r - return to main menu<br>
<br>
2. n, p, 1, enter, enter - create new partition<br>
3. t,L,b - (FAT32)<br>
4. a,1 - make bootable<br>
5. p - Print<br>
6. w - write the table to disk<br>

<h2>Format</h2>

losetup -d /dev/loop0<br>

<b>// comment : for new version of fdisk first sector of the partition is 2048 and the offset is equal 2048 x 512</b><br>
losetup /dev/loop0 /dev/my_usb -o 1048576<br>

mkdosfs -F32 /dev/loop0<br>
mount -t vfat /dev/loop0 my_dir<br>
cd my_dir<br>
mkdir boot<br>
cp kernel my_dir/boot/my_kernel<br>
umount /dev/loop0<br>
losetup -d /dev/loop0<br>


<h2>Edit akel.conf</h2>
<br>
In the first time you must create a file akel.conf and put inside the<br> folder boot. To edit the akel.conf you should put the following lines.<br>
<br>

Start with the title then press enter<br>

title = My os<br>

Line with the kernel Location<br>

kernel = /boot/my_kernel<br>

And if you have a file type ram disk place the following line<br>

initrd = /boot/my_initrd<br>

Or:<br>

module = /boot/my_module<br>

the disk and partitions start at zero in<br>
the akernelloader extended partitions start at 4<br>
do not forget to mark as bootable with fdisk partition<br> root that is inside the extended partition<br><br>

hd0 <- Disk number equal 0,1 ... <br>
(hd0, 0 <- partition number <br><br>
root = (hd0, 0 )<br>


<h2>Install Akernelloader</h2>
<br>
./akel -bs bootstrap /dev/my_usb<br>
./akel -ak akernelloader.bin /dev/my_usb<br>

<h2>Advice for compilation and utilization</h2>

If the akernelloader block during the execution try with a newer version of the compiler or package gnu binutils. Can also happen that the bootloader is correctly recorded in the usb if it does disconnect and reconnect.