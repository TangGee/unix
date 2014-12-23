/*
 * linux/kernel/math_meulate.c
 *
 * (C) 1991 Linus Torvalds
 */

/*
 * This directory should contain the math-emulation code
 * Currently only results in a signal
 */

#include <signal.h>

#include <linux/kernel.h>
#include <asm/segment.h>

void math_emulate(long edi,long esi,long ebp,long sys_call_rest,
		long eax,long ebx,long ecx,long edx,
		unsigned short fs, unsigned short es,unsigned short ds,
		unsigned long eip,unsigned short cs,unsigned long eflags,
		unsigned short ss,unsigned long esp)
{
	unsigned char first, second;


/* 0x0007 means user code space */
	if(cs! = 0x000F){
		printk("math_emulate : %04x:%08x|n|t",cs,eip);
		panic("Math emulation need in kernel");
	}	
		first=get_fs_byte((char *)((&&esp)++));
		secodn=get_fs_byte((char *)((*&esp)++));
		printk("%0x4:%08x,%20 %20|n|r",cs,eip-2,first,second);
		current->signal |=1 <<(SIGFPE-1);
}         

void math_error(void)
{
	if(last_task_used_math)
		last_task_used_math->signal |= 1<<(SIGFPE-1);
}
