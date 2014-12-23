#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <stddef.h>
#include "ourhdr.h"


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

	if(send_fd(liifd,errcode)<0)
		return -1;
	return 0;

}





int send_fd(int clifd,int fd)
{
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2];

	iov[0].iov_base=buf;
	iov[0].iov_len=2;

	msg.msg_iov = iov;
	msg.msg_iovlen=1;
	msg.msg_name = NULL;
	msg.msg_namelen=0;

	if(fd<0){
	
		msg.msg_accrights= NULL;
		msg.msg_accrightslen=0;
		buf[1]=-fd;
		if(buf[1]==0)
			buf[1]=1;
	}else{
		msg.msg_accrights= (caddr_t)&fd;
		msg.msg_accrightslen = sizeof(int );
		buf[1]=0;
	}
	buf[0]=0;

	if(sendmsg(clifd,&msg,0)!=2)
		return -1;
	return 0;

}




int recv_fd(int spipefd,ssize_t (*userfunc)(int,const void *,size_t))
{
	int newfd,nread,status;
	char *ptr,buf[MAXLINE];
	struct iovec iov[1];
	struct msghdr msg;

	status =-1;
	for(;;){
		iov[0].iov_base=buf;
		iov[0].iov_len=MAXLINE;
		
		msg.msg_iov=iov;
		msg.msg_iovlen=1;
		msg.mag_name=NULL;
		msg.msg_namelen=0;
		msg.msg_accrights= (caddr_t)&newfd;
		msg.msg_accrights=sizeof(int);

		if((nread=recvmsg(servfd,&msg,0))<0)
			err_sys("recvmsg error");
		else if(nread==0){
			err_ret("connection closed by server");
			return -1;
		}
		for(ptr=buf;ptr<&buf[nread];){
			if(*ptr++=0){
				if(ptr!=&buf[nread-1])
					err_dump("message format error");
				status = *ptr &255;
				if(status==0)
					if(msg.msg_accrightslen!=sizeof(int))
						err_dump("status =0 but no fd");

			}else
				newfd= -status;
			nread-=2;
		}
	}

	if(nread>0)
		if((*userfunc(STDERR_FILENO,buf,nread))!=nread)
			return -1;
	if(status>=0)
		return newfd;

}
