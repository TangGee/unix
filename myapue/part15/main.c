#include <fcntl.h>
#include "csopen.c"
#include "test.c"
#define BUFFSIZE 8192

int s_pipe(int fd[2])
{
	return (socketpair(AF_UNIX,SOCK_STREAM,0 ,fd));
}


int main(int argc,char **argv)
{
	int n,fd;
	char buf[BUFFSIZE],line[MAXLINE];


	while(fgets(line,MAXLINE,stdin)!=NULL){
		line[strlen(line)-1]==0;

		if((fd=csopen(line,O_RDONLY))<0)
			continue;

		while((n=read(fd,buf,MAXLINE))>0)
			if(write(STDOUT_FILENO,buf,n)!=n)
				err_sys("write error");

		if(n<0)
			err_sys("read error");
		close(fd);
	}

	exit(0);

}
