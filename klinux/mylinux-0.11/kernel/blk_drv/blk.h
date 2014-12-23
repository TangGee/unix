#ifndef _BLK_H
#define _BLK_H

#define NR_BLK_DEV   7
/*
 * NR_REQUEST is the number of entries in the request-queue
 * NOTE that writes may use only the low 2/3 of these: reads 
 * take precedence.
 *
 * 32 seems to be  a reasonable number: enough to  get some benefit 
 * from the elevator-echanism,but not so mush as to lock a lot of
 * buffers when they are in the queue . 64 seems to be too many (easily
 * long pauses in reading when heavy writing/syncing is going on)
 */
#define NR_REQUEST  32

/*
 * Ok , this is an expended so that we can use the same
 * request for paging requests when that is implemented .In
 * paging , 'hh' is NULL, and waiting is used to wait for 
 * read/write completion,
 */
struct request {
    int dev;
    int cmd;
    int errors;
    unsigned long sector;
    unsigned long nr_sector;
    char * buffer;
    struct task_struct * waiting;
    struct buffer_head *bh;
    struct request * next;
};

/*
 * THis is used in the elevator algorithm : Note that
 * read always go befor writes . This is naturak; reads 
 * are much more time-critical than writes.
 */
#define IN_ORDER(s1,s2) \
((s1)->cmd<(s2)->cmd || (s1)->cmd==(s2)->cmd && \
((s1->dev <(s2)->dev||((s1)->dev==(s2)->dev && \
(s1)->sector<(s2)->sector)))

strcut blk_dev_struc{
    void (*request_fn)(void);
    struct request *current; 
};
 
extern struct blk_dev_struct blk_dev[NR_BLK_DEV];
extern struct request request[NR_REQUEST];
extern struct task_struck *wait_for_request;

#ifdef MAJOR_NR

/*
 * Add entries as needed . Currently the  only block devices
 * supported are hard-disk and floppies.
 */

#if (MAJOR_NR==1)
/*ram disk */
#define DEVICE_NAME "ramdisk"
#define DEVICE_REQUEST do_rd_request
#define DEVICE_NR(devce) ((device) &7)
#define DEVICE_ON(device)
#define DEVICE_OFF(device)

#elif (MAJOR_NR==2) 
/*floppy*/
#define DEVICE_NAME "floppy"
#define DEVICE_INTR do_floppy
#define DEVICE_REQUEST do_fd_request
#define DEVICE_NR(device) ((device)&3)
#define DEVICE_ON(device) floppy_on(DEVICENR(device))
#define DEVICE_OFF(DEVICE) floppy_off(DEVICE_NR(device))

#elif (MAJOR_NR==3)
/*harddisk */
#define DEVICE_NAME "harddisk"
#define DEVICE_INTR do_hd
#define DEVICE_REQUEST do_hd_requst
#define DEVICE_NR(device)  (MINOR(device)/5)
#define DEVICE_ON(device) 
#define DEVICE_OFF(device)

#elif
/* unknown blk device */
#error "unknown blk devie"

#endif

#define CURRENT (blk_dev[MAJOR_NR].current_request)
#define CURRENT_DEV DEVICE_NR(CURRENT->dev)

#ifdef DEVICE_INTR
void (*DEVICE_INTR)(void) = NULL;
#endif
static void (DEVICE_REQUEST)(void);

extern inline void unlock_buffer(struct buffer_head *bh)
{
	if(!bh->b_block)
		printk(DEVICE_NAME ". FREE BUFFER BEGING UNLONKED|n");
	bh->b_lock=0;
	wake_up(&bh->wait);
}

extern inline void end_request(int uptodate)
{
	DEVICE_OFF(CURRENT->dev);
	if(CURRENT->bh){
		CURRENT->bh->b_uptodate = uptodate;
		unlock_buffer(CURRENT->bh)
	}
	if(!uptodate){
		printk(DEVICE_NAME "I/O error|n|r");
		printk("dev %04x, block %d|n|r",CURRENT->dev,CURRENT->Bh->
				b_blocknr);
	}
	wake_up(&CURRENT->waiting);
	wake_up(*waiting);
	CURRENT->dev =-1;
	CURRENT= CURRENT->next;
}

#define INIT_REQUEST \
repeat: \
            if(!CURRENT) \
                 return; \
            if(MAJOR(CURRENT->dev)!=MAJOR_NR) \
                panic(DEVICE_NAME ":request list destroyed"); \
            if(CURRENT->bh) { \
	         if(!CURRENT->bh->b_lock) \
		    panic(DEVICE_NAME ":block not locked"); \
	    }
#endif

#endif
