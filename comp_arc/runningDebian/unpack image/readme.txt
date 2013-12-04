How to easy split the Android boot image

I've put together a small utility undoing what mkbootimg does. Compared to similar utilities this also extracts the base addresses and command line parameters. It can also extract the kernel and root file system from a boot image embedded in a larger file, such as an update image.
Using this tool you can easily extract the ram root file system and kernel from the Android boot image.
Note that only parameters different from default are displayed, so if unmkbootimg does not display base address for example, you'll not have to specify it to mkbootimg neither.
About the mkbootimg build tool

For some phones, the boot image is built using non-standard offset(s). To rebuild your boot image again, you'll first have to recompile the build tool mkbootimg with the new offset(s). The unmkbootimg tool will tell you if this is needed and what offsets(s) to change.
How to recompile mkbootimg
Download

unmkbootimg version 1.2, gziped binary for GNU/Linux.
Feedback

XDA forum thread
Usage

$ unmkbootimg boot.img 
Kernel size 2419636
Kernel address 0x20008000
Ramdisk size 152656
Ramdisk address 0x21000000
Secondary size 0
Secondary address 0x20f00000
Kernel tags address 0x20000100
Flash page size 2048
Board name is ""
Command line "no_console_suspend=1"
Extracting kernel to file zImage ...
Extracting root filesystem to file initramfs.cpio.gz ...
All done.
---------------
To recompile this image, use:
  mkbootimg --kernel zImage --ramdisk initramfs.cpio.gz --base 0x20000000
            --cmdline 'no_console_suspend=1' -o new_boot.img
---------------
$ ls -l
totalt 5044
-rw-r--r-- 1 kuisma kuisma 2576384  9 sep 09.44 boot.img
-rw-r--r-- 1 kuisma kuisma  152656  9 sep 09.44 initramfs.cpio.gz
-rw-r--r-- 1 kuisma kuisma 2419636  9 sep 09.44 zImage
/By Mikael Q Kuisma