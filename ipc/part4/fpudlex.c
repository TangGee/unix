#include "unpipc.h"
#include "../lib/wrapunix.c"
#include "../lib/wrapstdio.c"

int main(int argc,char **argv)
{
	int fd[2];
	pid_t childpid;
	char c,p;
	ssize_t n;
	


	Pipe(fd);


	if((childpid=Fork())==0)
	{
		sleep(3);

		if((n=Read(fd[0],&c,1))!=1)
			err_quit("child error",n);
		printf("child %c",c);

		Write(fd[0],"c",1);
	}

	Write(fd[1],"p",1);

	if((n=Read(fd[1],&p,1))!=1)
		err_quit("p error");
	printf("parent %c",p);

	exit(0);

}

