#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "ourhdr.h"

int 
serv_listen(const char *name)
{
	int fd,len;
	struct sockaddr_un unix_addr;

	if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
		return -1;
	unlink(name);

	memset(&unix_addr,0,sizeof(unix_addr));
	unix_addr.sun_family=AF_UNIX;
	strcpy(unix_addr.sun_path,name);

	len=strlen(unix_addr.sun_path)+sizeof(unix_addr.sun_family);

	if(bind(fd,(struct sockaddr *)&unix_addr,len)<0)
		return -2;
	if(listen(fd,5)<0)
		return -3;

	return fd;
}
