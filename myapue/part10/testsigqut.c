#include "ourhdr.h"


static void sig_quit(int);



int main(void)
{
	sigset_t newmask, oldmask,pendmask;

	if(signal(SIGQUIT,sig_quit)==SIG_ERR)
		err_sys("can't catch SIGQUIT" );
	if(sigemptyset(&newmask)==-1)
		err_sys("sigemotyset error");
	if(sigaddset(&newmask,SIGQUIT)==-1)
		err_sys("sigaddset error");
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)==-1)
		err_sys("sigprocmask error");
	sleep(5);

	if(sigpending(&pendmask)<0)
		err_sys("sigpending error");
	if(sigismember(&pendmask,SIGQUIT))
		printf("pending SIGQUIT\n");
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)==-1)
		err_sys("SIG SETMASK error");

	printf("unblock SIGQUIT\n");
	sleep(20);

	printf("=============\n");

	exit(0);
}


static void sig_quit(int signo)
{
	printf("caught SIGQUIT\n");


	if(signal(SIGQUIT,SIG_DFL)==SIG_ERR)
		err_sys("can't reset SIGQUIT");
	
	return ;
}


