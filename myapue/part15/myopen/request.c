#include "ourhdr.h"
#include "open_fd.c"

/**
 * 要做的操作
 * 1 解析参数
 * 2 根据参数打开文件
 * 3 将文件描述符发送到fd
 */

static int buf_args(char *, int (*func)(int,char **));
static int cli_args(int,char **);
char errmsg[MAXLINE];
char * pathname;
int oflag;

#define CL_OPEN "open"
void
request(char * buf,int nread,int fd)
{
	int newfd;

	if(buf[nread-1]!=0){
		sprintf(errmsg,"request not null terminated: %*.*s\n",nread,nread,buf);

		send_err(fd,-1,errmsg);
		return ;
	}

	if(buf_args(buf,cli_args)<0){
		send_err(fd,-1,errmsg);
		return ;
	}

	if((newfd=open(pathname,oflag))<0){
		send_err(fd,-1,errmsg);
		sprintf(errmsg,"can't open: %s: %s\n",pathname,strerror(errno));;

		return ;
	}

	if(send_fd(fd,newfd)<0){
		err_sys("send_fd error");

	}
	close(newfd);
	



}

#define WHITE " "
#define MAXARGC 3
/**
 * 1 
 */
static int buf_args(char *buf, int (*func)(int,char **))
{
	char *ptr,*argv[MAXARGC];
	int argc;

	if(strtok(buf,WHITE)==NULL)
	{
		sprintf(errmsg,"can't strtok: %s use white: %s\n",buf,WHITE);
		return -1;
	}
	argv[argc=0]= buf;
	while((ptr= strtok(NULL,WHITE))!=NULL){
		if(++argc>MAXARGC-1)
			return -1;
		argv[argc]=ptr;
	}

	argv[++argc]=NULL;

	return ((*func)(argc,argv));

}

static int cli_args(int argc,char **argv)
{
	if(argc!=3||strcmp(argv[0],CL_OPEN)!=0){
		strcpy(errmsg,"usage:<pathname>,<oflag>\n");
		return -1;
	}

	pathname=argv[1];
	oflag=atoi(argv[2]);
	return (0);
	
		
}
