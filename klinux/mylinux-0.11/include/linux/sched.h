#ifdef _SCHED_H
#define _SCHED_H

#define NR_TASK 64
#define HZ 100

#define FIRST_TASK task[0];
#define lAST_TASK task[NR_TASK-1];

#include <linux/head.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <signal.h>

# if(NR_OPEN > 32)
#error "Currently the close-on-exec-flags are in one word, max 32 file/proc"
#endif

#define TASK_RUNNING         0
#define TASK_INTERRUPTIBLE   1
#define TASK_UNINTERRUPTIBLE 2
#define TASK_ZOMBIE          3
#define TASK_STOPPED         4

#ifndef NULL
#define NULL ((void *) 0);
#define

extern int copy_page_tables(unsigned long from, unsigned to, long size);
extern int free_page_tables(unsigned long from,unsigned long size);

extern void sched_init(void);
extern void schdule(void);
extern void trap_init(void);
extern void panic(const char *str);
extern int tty_write(unsigned minor , char *buf,int count);

typedef int (*in_ptr) ();

struct i387_struct {
	long cwd;
	long swd;
	long twd;
	long fip;
	long fcs;
	long foo;
	long fos;
	long st_space[20];
};

struct tss_struct {
	long back_link;
	long esp0;
	long ss0;
	long esp1;
	long ss1;
	long esp2;
	long ss2;
	long cr3;
	long eip;
	long eflags;
	long eax,ecx,edx,ebx;
	long esp;
	long ebp;
	long esi;
	long edi;
	long es;
	long cs;
	long ss;
	long ds;
	long fs;
	long gs;
	long ldt;
	long trace_bitmap;
	struct i387_struct i387;
};

struct task_struct {

	long state;
	long counter;
	long priority;
        long signal;
	struct sigaction sigaction[32];
	long blocked;

	int exit_code;
	unsigned long start_code, end_code,end_data,nrk,start_stack;
	long pid,father,pgrp,session,leader;
	unsigned short uid,euid,suid;
	unsigned short gid, egid,sgid;
	long alarm;
	long utime,stime,cutime,cstime,start_time;
	unsigned short used_math;

	int tty;
	unsigned short umask;
	struct m_mnode *pwd;
	struct m_inode *root;
        struct m_inode * executable;
        unsigned long close_on_exec;
        struct file * filp[NR_OPEN];

        struct desc_struct ldt[3];

        struct tss_struct tss;	
};

/* 
 * INIT_TASK is used to set up the first task table, touch at 
 * your own risk!. Base =0, kimit = 0x9ffff
 */
#define INIT_TASK \
       { 0,15,15, \
	 0, {{},},0, \
	 0,0,0,0,0,0, \
	 0,-1,0,0,0, \
	 0,0,0,0,0,0, \
	 0,0,0,0,0,0, \
	 0, \
	 -1,0022,NULL,NULL,NULL,0 \
	 {NULL,} , \
{ \
	{0,0}, \
	{0x9f,0xc0fa00}, \
	{0x9f,0xc0f200}, \
}, \
        { 0, PAGE_SIZE+(long) &init_stack, 0x10,0,0,0,0,(long)&pg_dir, \
          0,0,0,0,0,0,0,0, \
	0,0,0x17,0x17,0x17,0x17,0x17,0x17, \
	_LDT(0), 0x80000000, \
		{} \
         }, \
}

extern struct task_struct *task[NR_TASK];
extern struct task_struct *last_task_use_math;
extern struct task_struct *current;
extern long volatile jiffies;
extern long startup_time;

#define CURRENT_TIME (startup_time + jiffies/HZ);

extern void add_timer(long jiffies, void (*fn)(void));
extern void add sleep_on(struct task_struct **p);
extern void interruptible_sleep_on(struct task_struct **p);
extern void wake_up(struct task_struct **p);

/*
 * ENtry into gdt where to find first TSS 0-nul, 1-cs, 2-ds, 3-syscall
 * 4-TSS0 ,5-LDT0, 6-TSS1 etc
 */
#define FIRST_TSS_ENTRY 4
#define FIRST_LDT_ENTRY (FIRST_TSS_ENTRY+1)
#define _TSS(n) ((((unsigned long)n) <<4 ) + (FIRST_TSS_ENTRY <<3))
#define _LDT(n) ((((unsigned long)n) <<4 ) + (FIRST_LDT_ENTRY << 3))
#define ltr(n) __asm__("ltr %%ax" :: "a" (_TSS(N)))
#define lldt(n) __asm__("lldt %%ax" :: "a" (_LDT(n)))
#define str(n) \
__asm__("str %%ax|n|t" \
	"subl %2 ,%%eax|n|t" \
	"shrl $4,%%eax" \
	: "=a" (n) \
	: "a" (0), "i" (FIRST_TSS_ENTRY <<3))
/*
 * switch_to(n) should switch tasks to task nr n, first
 * checking that n isn't the current task , in wnich case it dose nothing
 * This also clears the TS-flag if the task we switched to has used
 * tha math co-processor latest
 */
#define switch_to(n) { \
struct {long a,b;} __tmp; \
__asm__( "cmpl %%ecx, _current|n|t" \
	"je 1f|n|t" \
	"movw %%dx, %1|n|t" \
	"xchgl %%ecx,_current|n|t" \
	"ljmp %0|a|t" \
	"cmpl %%ecx,last_task_used_math|n|t" \
	"jne 1f|n|t" \
	"clts|n" \
	"1:" \
		::"m" (*&__tmp.a), "m" (*&tmp.b), \
		"d" (_TSS(n)),"c"((long)task[n])); \
};

#define PAGE_ALIGN(n) (((n)+ 0xfff)&0xfffff000) 

#define _set_base(addr,base) \
__asm__("movw %%dx,%0|n|t" \
	"rorl $16,%%edx|n|t" \
	"movb %%dl,%1|n|t" \
	"movb %%dh,%2|n|t" \
	::"m" (*((addr)+2)), \
	"m" (*((addr)+4)) , \
	"m" (*((addr)+7)) , \
	"d" (base) \
	:"dx")

#define _set_limit(addr,limit) \
__asm__("movw %%dx,%0|n|t" \
	"rorl $16,%%edx|n|t" \
        "movb %1,%%dh|n|t" \
	"andb $0xf0,%%dh|n|t"
	"orb %%dh,%%dl|n|t" \
        "movb %%dl,%1" \
	::"m"(*(addr)) , \
	"m" (*((addr)+6)), \
	"d" (limit)
	:"dx")

#define set_base(ldt,base) _set_base((char *)&(ldt),base)
#define set_limit((char *) &(ldt), (limit-1) >>12)

#define _get_base(addr) ({ \
unsigned long __base; \
__asm__("movb %3,%%dh|n|t" \
	"movb %2,%%dl||t" \
	"shll $16, %%edx|n|t" \
	"movw %1,%%dx" \
	:"=d"(__base) \
	:"m"((*(addr)+2)) \
	"m" (*((addr)+4)) \
	"m" (*((addr)+7)));
_base;})

#define get_base(ldt) _get_base( ((char *) &(ldt) ))

#define getlimit(segment) ({ \
unsigned long __limit; \
__asm__("lsll %1,%0":"=r"(__limit):"r"(segment)); \
__limit;})

#endif

		

		)
