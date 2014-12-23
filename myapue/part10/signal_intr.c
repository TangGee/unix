#include "ourhdr.h"
/***
 *如果定一个SA_INTERRUPT 就禁止被终端的系统调用再次启动 这个signal的另一个版本
 
 */

Sigfunc * signal_intr(int signo,Sigfunc *func)
{
	struct sigaction act ,oldact;

	sigaction.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_falgs=0;

#ifdef SA_INTERRUPT
		act.sa_flags|= SA_INTERRUPT;
#endif
	if(sigaction(signo,&act,&oldact)<0)
		return (SIG_ERR);
	return (oldact.sig_handler);
}
