#include "unpipc.h"
#include "../lib/wrapstdio.c"

#define SEQFILE "seqfile"

void my_lock(int),my_unlock(int);


int main(int argc ,char **argv)
{
	int fd;
	long i,seqno;
	pid_t pid;
	ssize_t n;

	char line[MAXLINE+1];

	pid=getpid();
	if(
	(fd=open(SEQFILE,O_RDWR,FILE_MODE))<=0)
		err_sys("open error");
	printf("%d\n",fd);

	for(i=0;i<1000;i++)
	{
		my_lock(fd);
		//lseek 重新配置fd 设置便宜量为0
		Lseek(fd,0L,SEEK_SET);
		n=Read(fd,line,MAXLINE);
		line[n]='\0';
		n=sscanf(line,"%ld\n",&seqno);
		printf("%s: pid= %ld, seq# = %ld\n",argv[0],(long)pid , seqno);

		seqno++;

		snprintf(line,sizeof(line),"%ld\n",seqno);
		Lseek(fd,0L,SEEK_SET);
		Write(fd,line,strlen(line));

		my_unlock(fd);
	}
	exit(0);


}
