#include <signal.h>
#include "ourhdr.h"

static void sig_usr(int );

int main(void)
{
	if(signal(SIGUSR1,sig_usr)==SIG_ERR)
		err_sys("signal error");
	if(signal(SIGUSR2,sig_usr)==SIG_ERR)
		err_sys("signal error");


	for(;;)
	pause();

}

static void sig_usr(int signo)
{
	if(signo==SIGUSR1)
		printf("received SIGUSR1");
	else if(signo==SIGUSR2)
		printf("received SIGUSR2");
	else
		err_dump("receive signal %d\n",signo);
}

