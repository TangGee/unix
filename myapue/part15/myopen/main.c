#include "ourhdr.h"
#include "csopen.c"
#include "open_fd.c"

int s_pipe(int fd[2])
{
	return (socketpair(AF_UNIX,SOCK_STREAM,0 ,fd));
}


int main(int argc,char **argv)
{
	
	int n,fd;
	char line[MAXLINE];

	while(fgets(line,MAXLINE,stdin)!=NULL){
		n=strlen(line);
		line[n-1]=0;

		if((fd=csopen(line,O_RDONLY))<0)
			continue;

		while((n=read(fd,line,MAXLINE))>0){
			if(write(STDOUT_FILENO,line,n)!=n)
				err_sys("write error");
		}

		if(n<0)
			err_sys("read error");

		close(fd);




	}

	exit(0);
}
