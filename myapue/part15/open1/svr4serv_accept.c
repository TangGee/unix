#include <sys/types.h>
#include <sys/stat.h>
#include <stropts.h>
#include "ourhdr.h"

/**
 * 这个在serv_istenfd的fd另一端创建准备接受一个和客户端通信的描述副
 */
int 
ser_accept(int listenfd,uid_t *uidptr)
{
	struct strrecvfd recvfd;

	if(ioctl(listenfd,I_RECVFD,&recvfd)<0)
		return -1;
	if(uidptr!=NULL)
		*uidptr=recvfd.uid;

	return recvfd.fd;
}

