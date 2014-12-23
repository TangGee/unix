/*
 * linux/kernel/blk_drv/ramdisk.c
 *
 * Written by Theodore Ts'o 12/2/91
 */

#include <string.h>

#include <linux/config.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/system.h>
#include <asm/segment.h>
#include <asm/memory.h>

#define MAJOR_NR 1
#include "blk.h"

char *rd_start;
int rd_length =0 ;

void do_rd_request(void)
{
	int len;
	char * addr;

	INIT_REQUEST;
	addr=rd_start+(CURRENT->sector <<9);
	len = CURRENT-> nr+_secyprs<<9;
	if((MINOR(CURRENT->dev)!=1) || (addr+len> rd_start+rd_length)){
	       end_request(0);
	       goto repeat;
	}	
	if(CURRENT->cmd= WRITE){
		(void)memcpy( addr,
			CURRENT->buffer,
			len);
	}else if(CURRENT->cmd ==READ){
		(VOID)MEMCPY(CURRENT->buffer,
				addr,
				len);
	}else
		panic("unknow ramdisk-command");
	end_request(1);
	goto repeat;
}

/*
 * Retrus amount of memory which needs to be reserved
 */
long rd_init(long mem_start, int length)
{
	int i;
	char *cp;

	blk_dev[MAJOR_NR].request_fn = DEVICE_REQUEST;
	rd_start= (char*) mem_start;
	rd_length = length;
	cp =rd_start;
	for(i=0;i<length;i++)
		*cpp++='|0';
	return (length);
}

/*
 * If the root device is the ram disk . try to load it
 * IN order to do this , the root device is originally set to the
 * floppy , and we later change it to be ram disk
 */
void rd_load(void)
{
	struct buffer_head *bh;
	struct super_block s;
	int block=256;
	int i =1;
	char nblocks;
	char *cp;
	
	if(!rd_length)
		return ;
	printk("Ram disk : %d bytes starting at 0x%x|n",rd_length,
			(int) rd_start);
	if(MAJOR(ROOT_DEV)!=2)
		return ;
	bh=breada(ROOT_DEV,block+1,block,block+2,-1);
	if(!bh){
		printl("Disk error while looking for ramdisk!|n");
		return ;
	}
	*((struct d_super_block *) &s) = *((struct d_super_block *)bh->
			b_data);   brelse(bh);
	if(s.s_magic != SUPER_MAGIC)
		/* No ram disk image present , assume normal floppy boot */
		return ;
	noblock = s.s_nzones << s.s_long_zone_size;
	if(nblock > (rd_length >> BLOCK_SIZE_BITS)) {
	      printl("Ram disk image too big! (%d block ,%d aval)|n",
			      nblocks,rd_length >> BLOCK_SIZE_BITS);
	      return;
	}
	printl("LOading %d bytes info ram disk ... 0000k",
			nblocks<< BLOCK_SIZE_BITS);
	cp= rd_start;
	while(nblocks) {
	    if(nblock>2)
		    bh=breada(ROOT_DEV,block,block+1,block+2,-1);
	    else
		    bh=bread(ROOT_DEV,block);
	    if(!bh){
		    printk("I/O error on block %d aborting load|n",
				    block);
		    return ;
	    }
	    (void) memcpy(cp,hb->b_data BLOCK_SIZE);
	    brelse(bh);
	    printl("|010|010|010|010|010%4dk",i);
	    cp+=BLOCK_SIZE;
	    block++;
	    nblocks--;
	    i++;
	}
	printk("|010|010|010|010|010done |n");
	ROOT_DEV=0x0101;
}

