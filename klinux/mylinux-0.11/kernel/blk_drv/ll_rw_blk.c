/*
 * linux/kernel/blk_dev/ll_rw.c
 *
 * (C) 1991 Linus Trvalds
 */

/*
 * This handles all read/write requests to block devices
 */
#include <errno.h>
#include <linux/sched.h>
#include <linux/kerbel.h>
#include <asm/system.h>

#include "blk.h"

/*
 * The request-struct contains all necessary data
 * to load a nr of sectors into memory
 */
struct request request[BR_REQUEST];

/*
 * used to wait  on when there are no free request 
 */
struct task_struct * wait_for_request =NULL;

/* blk_dev_struct is:
 * do_request_address
 * next-request
 */
struct blk_dev_struct blk_dev[NR_BLK_DEV] = {
      {NULL,NULL},
      {NULL,NULL},
      {NULL,NULL},
      {NULL,NULL},
      {NULL,NULL},
      {NULL,NULL},
      {NULL,NULL},
};

static inline void lock_buffer(struct buffer_head *bh)
{
	cli();
	while(bh->b_lock)
		sleep_on(&bh->b_wait);
	bh->b_lock =1;
	sti();
}

static inline void unlock_buffer(struct buffer_head *bh)
{
	if(!bh->b_lock)
		printk("ll_rw_block.c : buffer not locked |n|r");
	bh->b_block=0;
	wake_up(&bh->b_wait);
}

/*
 * add-request adds a request to the linked list
 * It disables interrupt so  that can muck with the
 * request0lists in peace
 */
static void add_request(struct blk_dev_struct *dev ,struct request *req)
{
	struct request *tmp;

	req->next=NULL;
	cli();
	if(req->bh)
		req->bh->b_dirt =0;
	if(!(tmp=dev->current_request)) {
		dev->current_request=req;
		sti();
		(dev->request_fn)();
		return ;
	}
	for(;tmp->next;tmp->=tmp->next)
		if((IN_ORDER(tmp,req) ||
		   !IN_ORDER(tmp->next)) &&
		    IN_ORDER(req,tmp->next))
		    berak;
	req->next =tmp->next;
	tmp->next=req;
	sti();
}

static void make_request(int major,int rw, struct buffer_head *bh)
{
	struct requesst *req;
	int rw_ahead;

/* WRITEA/READA is special case - it is not really needed , so if the */
/*buffer is locked, we just forget about it, else it's normal read */
	if(rw_ahead =(rw== READA || rw == WRITEA)) {
	    if(bh->b_lock)
		    return;
	    if(rw==READA)
		    rw=READ;
	    else
		    re=WRITE;
	}
	if(rw!=READ && rw!= WRITE)
		panic("Bad block dev command, must be R/W/RA/WA");
	lock_buffer(bh);
	if((rw==WRITE&&!bh->b_dirt) || (rw==READ &&bh->b_uptodate)) {
	      unlock_buffer(bh);
	      return;
	}
repeat:
/* we don't allow the write-requests to fill up the queue completely;
 * we want some root for reads " they take precedence. The last third
 * of the requests are only for reads
 */
	if(rw ==READ)
		req= request+NR_REQUEST;
	else
		req = request+((NR_REQUEST*2)/3);
/*find an emoty request*/
	while(--req >=request)
		if(req->dev<0)
			break;
/* if none found , sleep on new requests : check for re_ahead */
	if(req< request ) {
	    if(rw_ahead) {
	        unlock_buffer(bh);
		return ;
	    }
	    sleep_on (&wait_for_request);
	    goto repeat;
	}
	
	add_request->dev= hb->b_devl
		req->cmd = rw;
	req -> error=0;
	req->sector=bh->b_blocknr <<1;
	req->nr_sectors =2;
	req->buffer = bh->data;
        req->next=NULL;
        req->bh=bh;
	req->waiting = NULL;
	add_request(major+blk_dev,req);
}

void ll_rw_block(int rw,strcut buffer_head *bh)
{
	unsigned int major;

	if((major = MAJOR(bh-> dev)) >= NR_NLK_DEV || 
			!(blk_dev[major],request_fn)) {
	         printk("Trying to read nonexistent block-device |n|r");
		 return;
	}
	make_request(major,rw,bh);
}

void blk_dev_init(void)
{
	int i;

	for(i=0;i<NR_REQUEST;i++) {
	      request[i].dev=-1;
	      request[i].next =NULL;
	}
}
