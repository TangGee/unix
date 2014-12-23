#include "ourhdr.h"

int main(void)
{
	pid_t pid;

	printf(" before fork pid= %d\n", getpid());

	if((pid=fork())==-1)
		err_sys("fork error\n");
	else if(pid==0)
	{
		execl("/bin/sh" ,"sh","-c" ,"/bin/date",(char *)0);
		printf("child pid=%d\n", getpid());
		_exit(127);
		
	}
	else
		printf("1\n");

	printf("i don't know  pid=%d \n",getpid());
		
}
