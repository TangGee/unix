!
!SYS_SIZE is the number of clicks(16 byte) to be loaded
!0x3000 is 0x30000 byte=196kB , more than enough for current
!versuibs of linux
!SYS_SIZE是要加载的系统模块长度，单位是节，16字节为1节，0x3000恭维0x30000字节=
!196kB（若以1024字节为1kB计，则应该为192kB），对于当前版本空间已经足够了，这里
!的感叹号或;是注释语句的开始
!
!下面等好或EQU用户定义表示符号所代表的值，可以成为符号常量。这个常量知名编译链接后system模块大小。这个等是”SYSSZE=0x3000“预案来由linux/Makefile中第92行上的语句动态自动产生。但从Linux 0.11版本开始就直接在这里给出了一个最大默认值。原来参见
!自动生成语句还没有被删除，参见程序5-1中第92行的说明，当前值为0x8000时表示内核
!内核为512KB  1000 0000 0000 0000 
SYSSIZE = 0x3000
!
!   bootsect.s    (C)1991 Linus Torvalds
!
!bootsect.s is loaded of 0x7c00 by the bios-startup routines,and movs iself out
!of the way to address 0x90000 and jumps there 1001 0000 0000 0000 1
