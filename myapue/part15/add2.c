#include <signal.h>
#include "ourhdr.h"

static void sig_pipe(int );

/*
int s_pipe(int fd[2])
{
	return pipe(fd);
}
*/


int s_pipe(int fd[2])
{
	return (socketpair(AF_UNIX,SOCK_STREAM,0 ,fd));
}
int main(void)
{
	int n,fd[2];
	pid_t pid;
	char line[MAXLINE];

	if(signal(SIGPIPE,sig_pipe)==SIG_ERR)
		err_sys("signal error");
	if(s_pipe(fd)<0)
		err_sys("pipe error");
	if((pid=fork())<0)
		err_sys("fork error");
	else if(pid>0){
		close(fd[1]);

		while(fgets(line,MAXLINE,stdin)!=NULL){
			n=strlen(line);
			line[n]=0;
			if(write(fd[0],line,n)!=n)
				err_sys("write error");
			if((n=read(fd[0],line,MAXLINE))<0)
				err_sys("read error from pipe");

			if(n==0){
				err_msg("child close pipe");
				break;
			}

			line[n]=0;
			if(fputs(line,stdout)==EOF)
				err_sys("fputs error");

		}

		if(ferror(stdin))	
			err_sys("fgets error on stdin");
		exit(0);
	}else{
		close(fd[0]);
		if(fd[1]!=STDIN_FILENO)
		{
			if(dup2(fd[1],STDIN_FILENO)!=STDIN_FILENO)
				err_sys("fgets error on stdin");

		}
		if(fd[1]!=STDOUT_FILENO){

			if(dup2(fd[1],STDOUT_FILENO)!=STDOUT_FILENO)
				err_sys("fgets error on stdout");
		}
		if(execl("./add2","add2",NULL)<0)
			err_sys("execl error");
	}
		
}


static void sig_pipe(int signo)
{
	printf("----");
	exit(1);
}



/*
static void sig_pipe(int fd[2])
{
	return (socketpair(AF_UNIX,SOCK_STREAM,0,fd));
}
*/
