#include "ourhdr.h"


/**
 * 这是signal的一个版本 它尽可能使用被信号中断的服务自动启动 
 */

Sigfunc * signal(int signo,Sigfunc *func)
{
	struct sigaction act,oldact;

	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	if(signo==SIGALRM){
#ifdef SA_INTERRUPT
		act.sa_flags|= SA_INTERRUPT;
#endif
	}else{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}

	if(sigaction(signo,&act,&oldact)<0)
		return (SIG_ERR);
	return (oact.sa_handler);

}
