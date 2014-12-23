#include "ourhdr.h"
#include <stropts.h>
int send_fd(int spipefd,int filedes);
int send_err(int spipefd,int status,const char *errmsg);
int recv_fd(int spipefd,ssize_t (*userfunc)(int,const void *,size_t));


int send_err(int clifd,int errcode,const char *msg)
{
	int n;

	if((n=strlen(msg))>0)
		if(write(clifd,msg,n)!=n)
			return -1;
	if(errcode>=0)
		errcode=-1;

	if(send_fd(clifd,errcode)<0)
		return -1;
	return 0;

}


int send_fd(int clifd,int fd)
{
	char buf[2];

	buf[0]=0;

	if(fd<0){ //errcode=-1
		buf[1]=-fd;
		if(buf[1]==0)
			buf[1]=1;
	}else{
		buf[1]=0;
	}

	if(write(clifd,buf,2)!=2)
		return -1;
	if(fd>=0)
		if(ioctl(clifd,I_SENDFD,fd)<0)
			return -1;
	return 0;
}


/**
 * 当接受一个描述符的时候  ioctl的第三个参数是一个strrecvfd的指针
 * struct strrecvfd{
 *     int fd;
 *     uid_t uid;
 *     gid_t gid;
 *     char fill[8];
 * };
 */





int recv_fd(int servfd,ssize_t (*userfunc)(int,const void *,size_t))
{
	int newfd,nread,flag,status;
	char *ptr,buf[MAXLINE];
	struct strbuf dat;
	struct strrecvfd  recvfd;

	status=-1;
	for(;;){
		dat.buf =buf;
		dat.maxlen=MAXLINE;
		flag=0;
		if(getmsg(servfd,NULL,&dat,&flag)<0)
			err_sys("getmsg error");
		nread=dat.len;
		if(nread==0){
			err_ret("connection closed by server");
			return -1;
		}

		for(ptr=buf;ptr<&buf[nread];){
			if(*ptr++==0){
				if(ptr!=&buf[nread-1])
					err_dump("message format error");
				status=*ptr&255;
				if(status==0){
					if(ioctl(servfd,I_RECVFD,&recvfd)<0)
						return -1;
					newfd = recvfd.fd;

				}else
					newfd=-status;
				nread-=2;
			}
		}

		if(nread>0)
			if((*userfunc)(STDERR_FILENO,buf,nread)<0)
				return -1;
		if(status>=0)
			return newfd;




	}

}

