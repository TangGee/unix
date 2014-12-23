#
##if you want the ram-disk device,define this to be the
#size in block
#
#如果你要使用RAM盘设备的话就定义块的大小，这里默认使用RAMDISK没有定义（注释掉了
#）否则gcc编译时会带有选项-DRAMDISK=512,参见第18行
RAMDISK =#-DRAMDISK

AS86    =as86 -0 -a  #8086汇编编译编译器和连接器，见列表后的介绍，带参数分别是
LD86    =ld86 -0     #-0生成8086目标程序，-a生成与gas和gld部分兼容的代码

AS      =as          #gun汇编编译器和连接器，见列表后的介绍
LD      =gld         
#下面是GUN连接器gld运行时用到的选项。含义是： -s输出文件中省略的所有序号信息：
#-x删除所有的局部标号： -M 表示需要在标准的输出设备（显示器）打印链接映像（link
#map），是指由链接程序长生的一种内存地址映像，其中列出了程序段装入的内存中的位
#置信息。具体来讲如如下信息：
#1目标文件及符号信息映射到内存中的位置：
#2公共符号如何放置
#3链接包含的所有文件成员及成员引用的符号
LDFLAGS = -s -x -M

#gcc是GNU c程序编译器。对于UNIX类的脚本script程序语言，在引用定义的标识符时，
#需要在前边加上$符号并用括号括住标识符号 
CC =gcc $(RAMDISK)   

#下面制定gcc使用的选项。前一行最后的‘\'符号表示下一行是续行。选项含义为-Wall打
#印所有警告信息;-0对大妈进行优化。‘-f标志’指定与机器无关的编译标志。其中
#-fstrength-reduce用于优化循环语句;-fomit-frame-pointer指明无需帧指针的函
#数不要把指针保留在寄存器中。这样在函数中可以避免对帧指针的操作和维护。
#-mstring-insns是linus在学习gcc编译器时为gcc增加的选项，用于gcc-1.40在复制结构等操作时使用386 CPU的字符串指令，可以去掉。
CFLAGS =-Wall -0 -fstrength-reduce -fomiy-frame-pointer \
-fcombine-regs -mstring-insns

#下面cpp是gcc的前（预）处理程序。前处理器用于包含程序中的宏替换处理，条件编译处理以及包含进制定的文件的内容，即把使用‘#include’制定的文件包含进来。源程序文件
#中所有以符号‘#’开始的行均需要由前处理器进行处理。程序中所有‘#define’定义的宏
#店铺会使用其定义的部分替换掉。程序所有‘#if’，‘#ifdef’，‘#ifndef’和‘#enfis’等
#条件判断别行用于确定是包含制定范围的语句
#‘-nostdinc -Iinclude’含义是不要搜索标准头文件，即不用系统/usr/include/目录下的头文件，而是使用‘-I’选项制定的目录或者是在当前目录搜索源文件
CPP =cpp -nostdinc -Iinclude

#
#ROOT——DEV sopecifile the default root-device wen making te image
#This can be either FLOPPY， /dev/xxxx or empty，in whiich case t
#the default of /dev/hd6 is used by ‘build’
#
#ROOT_DEV制定创建内核映像文件所以需要要的默认跟文件系统所在的设备，这可以是软盘
#/dev/xxxx或者干脆为空着，空着是build程序（在tools/目录中）即使使用默认值
#/devhd6
#这里/dev/hd6对应第二个应哦按的地一个分区。这是Linus'开发Linux内核时自己的
#机器上跟文件系统所在的分区位置
ROOT_DEV=/dev/hd6

#下面是kernel目录。mm目录和fs目录所产生的目标代码文件。为了方便引用这里将
#他们用ARCHIVES（归档文件）标识符表示
ARCHIVES=kernel/kernel.o mm/mm.o fs/fs.o

#块和字符设备文件。‘.a’表示该文件是个归档文件，也包含有许多可执行二进制代码
#子程序集合的哭文件，通常都是GNU的ar程序生成。ar是GNU的二进制文件处理程序，
#用于创建，修改以及从归档文件中抽取文件
DRIVERS =kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a
MATH =kernel/math/math.a          # 这是数学运算库文件
LIBS =lib/lib.a               #这是lib/目录下的文件所编译生产的通用库

#下面是make老式的隐式后缀规则，该行指示make利用下面的命令将所有‘.c’文件编辑
#生成'.s'汇编程序。‘：’表示下面是该规则的命苦。整句表示让gcc采用前面CFLAGS
#所指示的选项以及使用include/目录下的头文件。是当地编译后不进行汇编（-S）就
#停止，从而产生与输入的各C文件对应的汇编语言形式的代码文件。默认情况下所有
#产生的汇编程序文件是原C文件名去掉‘。c’后在加上‘.s’后缀。‘-o’比赛欧式气候是输出文件的形式。其中‘$*.s’（或$@）是自动目标变量，‘￥《’代表地一个先决条件
#这里即是符合条件的*.c的文件
#下面这3个不同的规则分别用于不同的操作要求。若目标是.s文件，而源是.c后文件
#则会使用地一个规则，若目标是.o而源文件是.s则使用第2个规则：若目标是.o文件
#而源文件是.c则可直接使用第3个规则
.c.s
     $(CC) $(CFLAGS) \
	     -nostdinc -Include -S -o $*.s $<
#表示所有.s汇编程序文件编译成.o目标文件。证据表示gas编译器将汇编程序编译成
#.o目标文件按 -c表示编译或汇编，但不进行连续操作
.s.o
    $(AS) -c -o $*.o $< 

#类似上面，*.c文件-->×*.o文件。整句表示使用gcc将C语言文件编译成目标文件爱你
#但不链接
.c.o
    $(CC) $(CFLAGS) \
    -nostdinc -Iinclude -c -o $*.s $<

#下面“”all“ 表示创建Makefile所知的最顶层的目标。这里即是Image文件。这里生成的
#Image文件即是引导启动盘映像文件bootimage，若将其写入软盘就可以使用该软盘引导
#LInux系统了。在Linux下将Image写入引导软盘额命令见46行。DOS系统下可以使用软件
#rawrite.exe
all:  Image

#说明目标（Image文件）是由毛好后面的4个元素产生，分别是boot/目录西安的setup文件
#tools/目录中的system和build文件。42-43行这是执行的命令。42行表示使用tools目录
#下的build工具程序将bootsect setup和system文件以$(ROOT_DEV) 为根文件系统设备组装成内核映像文件Image 第43行的sync同步命令是迫使缓冲数据立即写盘并更新超级块
Image: boot/bootsect boot/setup tools/system tools/build
	tools/build boot/bootsect boot/setup tools/system ${ROOT_DEV} > Image
	sync
#表示disk这个目录要由Image产生。dd为UNIX标准命令：复制一个文件，根据选项所进行
#转换额格式化。bs=表示一次读/写的子结束。if=表示输入的文件，of表示输出的文件。
#这里/dev/PS0是指地一个软盘驱动器设备文件按，在linux系统下使用/dev/fd0
disk: Image
	dd bs=8192 if=Image of=/dev/PS0

tools/build: tools/build.c    #由tools目录下的build.c程序生成执行程序build
	$(CC) $(CFLAGS) \
	-o tools/build tools/build.c #编译生成执行程序build的命令

boot/head.o: boot/head.s  
	#利用上面给出的so规则生成head.o目标文件

#表示tools目录中的system文件要由毛好右边所列的元素生成 51-61 汗是生成system的
#命令。最后的>System.map表示gld需要将链接映像重定向为System.map文件中。
#关于System.map文件参见注释后的说明
tools/system: boot/head.o init/main.o \
	$(ARCHIVES) $(DRIVERS) $(MATH) $(LIBS) 
	$(LD) $(LDFLAGS) boot/head.o init/main.o \
	$(ARCHIVES) \
	$(DRIVERS) \
	$(MATH) \
	$(LIBS) \
	-o tools/system > System.map

#数学协处理器函数文件math.a由64行上的命令实现：进入kerenl/math目录：运行make
#工具程序。
kernel/math/math.a:
	(cd kernel/math; make)

kernel/blk_drv/blk_drv.a:
	(cd kernel/blk_drv; make)

kernel/chr_drv/chr_drv.a:
	(cd kernel/chr_drv; make)

kenel/kernel.o
        (cd kernel;make)

mm/mm.o:
	(cd mm; make)

fs/fs.o:
	(cd fs; make)

lib/lib.o:
	(cd lib; make)

boot/setup: boot/setup.s
	$(AS86) -o boot/setup.o boot/setup.s
	$(LD86) -o boot/setup boot/setup.o

boot/bootsect: boot/bootsect.s
	$(AS86) -o boot/bootsect.o boot/bootsect.s
	$(AS86) -o boot/bootsect  boot/bootsect.o

#下面92-95行的作用是在bootsect.s文本程序开始处添加一行有关system米快长度的信息
#把在system模块加载到内存期间用于知名系统模块的长度。添加该行信息的方法是首先
#生成只含有”SYSSIZE =system文件实际长度“一行信息的tmp.s文件，然后将bootsect.s
#文件，然后将bootsect.s文件添加在其后。去的system长度的方法是利用命令ls对编译
#生成的system模块文件进行长列表显示，使用grep命令去的列表行上下文年间子结束字段
#信息，并向保存在tmp.s临时文件中。cut命令用于剪切字符串，tr用于取出行为的回车符
#其中（实际长度+！5）/16用于获得‘节’表示信息的长度信息
#1字节=16字节
#注意这里LInux 0.11 之前的内核版本获取system模块长度并添加到bootsect.s 程序中
#的使用方法。从0.11从0.11版本的内核开始已不使用这个方法，而是在bootsect.s程序
#开始处给粗system模块的最大默认长度值。因此这些规则现在不起作用
tmp.s: boot/bootsect.s tools/system
	(echo -n "SYSSIZE = (":ls -l tools/system | grep system \
		| cut -c25-31 | tr '\0.12' ' '; echo "+15)/16") >tmp.s
	cat boot/bootsect.s >> tmp.s

clean:
	#当程序执行‘make clean’时，就会执行98-103行的命令，出去所有编译链接生成
	#的文件 ‘rm’ 是文件删除命令 -f是忽略不存在的文件，并且不显示删除信息
	rm -f Image System.map tmp_make core boot/bootsect boot/setup
	rm -f init/*.o tools/system tools/bulid tools/*.o
	(cd mm; make clean)
	(cd fs; make clean)
	(cd kernel; make clean)
	(cd lib; make clean)

#该规则将首先执行上面的clean规则然后对linux/目录进行压缩生成backup.Z压缩文件。
#‘cd 。。’表示推到Linux/的上以及父目录： ‘tar cf -linux’ 表示对linux/目录执行
#tar归档 欻地给压缩程序compress并将压缩程序输出成backup.Z文件按
bcakup: clean
	(cd .. ; tar cf - linux | compress -> backup.Z )
	sync

dep:
	#该目标或规则用于产生个文件间的以来关系。传见这些依赖关系为了make命令用
	#它来去定是否需要重建 一个目标对象。比如当某个热头文件被改动过后，make
	#就能通过生成的以来关系冲洗编译与该头文件有关*.c文件按，具体方法如下：
	#使用字符串编辑程序sed对Makefile文件（这里即文本文件）处理，输出为删除
	#Makefile文件中‘###Dependenc.ies’行后面的所有行，即删除下面从118开始到
	#文件末的所有行，即删除从118行开始到文件末尾的是偶有行，并生成一个默认的        #临时文件tmp_make（也即110行的作用）。然后对制定目录（init）下的没一个C
	#文件（其实只有一个文件main.c）执行gcc预处理操作。标志‘-M’高所预处理程序        #cpp输出描述每个目标文件相关性的规则，并且这些规则符合make语法。对于每
	#于每个源文件，预处理程序会输出一个规则，其结果形式就是相应的源程序文件
	#的目标文件名加上其依赖关系，即该源文件中包含的所有头文件列表。
	#然后把预处理的结果都添加到临时文件tmp_make中，最后将该临时文件复制成新
	#makefile文件
	#111行上的$$i实际上是$（$i） 这里$i 是这句前面的shell变量i
          
	  sed '/\#\#\#Dependencies/q' < Makefile >  tmp_make
	  (for i in init/*.c; do echo -n "init/";$(CPP) -M $$i;done) >> tmp_make
           cp tmp_make Makefile
	   (cd fs; make dep)
	   (cd kernel;make dep)
	   (cd mm;make dep)

### Dependencies:
init/main.o : init/main.c include/unistd.h include/sys/sys.stat.h \
	include/sys/types.h include/sys/time.h include/sys/utsname.h \
	include/utime.h include/time.h include/linux/tty.h include/termios.h \
	include/linux/sched.h include/linux/head.h include/linux/fs.h \
include/linux/mm.h include/signal.h include/asm/system.h include/asm/io.h \
        include/stddef.h include/stdarg.h include/fcntl.h


	
