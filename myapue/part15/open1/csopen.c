#include "ourhdr.h"
#include <sys/uio.h>
#include "bsdcli_conn.c"

#define CS_OPEN "/home/tlinux/open"
#define CLI_OPEN "open"
int 
csopen(char *name,int oflag)
{
	char buf[10];
	static int csfd=-1;
	struct iovec iov[3];
	int len;

	if(csfd<0){
		if((csfd=cli_conn(CS_OPEN))<0)
			err_sys("cli_conn error");
	}

	sprintf(buf," %d",oflag);
	iov[0].iov_base=CLI_OPEN " ";
	iov[0].iov_len=strlen(CLI_OPEN)+1;
	iov[1].iov_base=name;
	iov[1].iov_len=strlen(name);
	iov[2].iov_base=buf;
	iov[2].iov_len=strlen(buf)+1;
	len=iov[0].iov_len+iov[1].iov_len+iov[2].iov_len;

	if(writev(csfd,iov,3)<0)
		err_sys("writev error");

	return (recv_fd(csfd,write));





}
