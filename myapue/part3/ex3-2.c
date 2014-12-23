#include "apue.h"
#include <fcntl.h>
#include <errmethod.h>
int copyfd(int);

int main(int argc,char **argv)
{
	char buf[1024];
	int fd,fd1,n;
	if(argc!=2)
		err_quit("usage <pathname>");
	if((fd=open(argv[1],O_CREAT|O_RDWR,FILE_MODE))==-1)
		err_sys("open error");

	if(write(fd,"1",1)!=1)
		err_sys("write error");

	fd1=copyfd(fd);

	if((n=read(fd,buf,1024))<0)
		err_sys("read error");
	buf[n]='\0';

	printf("===%s",buf);

	

}


int copyfd(int o_fd)
{
	char buf[100];
	int n_fd;
	sprintf(buf,"/dev/fd/%d",o_fd);
	if((n_fd=open(buf,O_CREAT|O_RDONLY,FILE_MODE))==-1)
		err_sys("open error");

	return n_fd;
}

