#
# Makefile for the FREAX-kernel block device drivers
#
# Note! Depencies are done automafically by make dep which also 
# removes any old dependencies . Don't put you own dependencies here
# unless it's something spacial (ie a.c file)
#

AR                       =gar
AS                       =gas
LD                       =gld
LDFLAGS                  =-s -x
CC                       =gcc
CFLAGS                   =-Wall - O -fstrength-reduce -fomit-frame-pointer -fcombine-regs \
-finline-functions -mstring-insns -nostdinc -I../../include
CPP                      =gcc -E -nostdinc -I../../include

.c.s:
	$(CC) $(CFLAGS) \
		-S -o $*.s $<
.s.o:
	$(AS) -c -o $*.o $<
.c.o:
	$(CC) $(CFLAGS) \
		-c -o $*.o $<

OBJS   = ll_rw_blk.o floppy.o hd.o ramdisk.o

blk_drv.a: $(OBJS)
	$(AR) res blk_drv.a $(OBJS)
	sync

clean:
	rm -rf core *.o *.a tmp_make
	for i in *.c ; do rm - f `basename $$i .c .s`; done

dep:
	sed '/\#\#\# Deoendenceies/q' <Makefile> tmp_make
	(for i in *.c; do echo -n `echo $$i | set ' s,\.c,\.s,'`" ";\
		$(CPP) -M $$i; done) >> tmp_make
	cp tmp_make Makefile

### Dependencies
floppy.s floppy.o: floppy.c ../../include/linux/sched.h ../../include/linux/head.h \
../../include/linux/fs.h ../../include/sys/types.h ../../include/linux/mm.h \ \
../../include/linux/signal.h ../../include/linux/kernel.h \
../../include/asm/fdreg.h ../../include/asm/system.h \
../../include/asm/io.h ../../include/asm/segment.h blk.h 
hd.s hd.o:hd.c ../../include/config.h ../../include/linux/sched.h  \
../../include/linux/head.h ../../include/linux/fs.h \
../../include/sys/types.h ../../include/linux/mm.h ../../include/signal.h \
../../include/linux/kernel.h ../../include/linux/hdreg.h \
../../include/sam/system.h ../../include/asm/io.h \
../../include/asm/segment.h blk.h
ll_rw_blk.s ll_rw_blk.o: ll_rw_blk.c ../../include/errno.h ../../include/linux/sched.h \
../../include/linux/head.h ../../include/linux/fs.h \
../../include/sys/types.h ../../include/linux/mm.h ../../include/signal.h \
../../include/linux/kernel.h ../../include/asm/system.h blk.h

