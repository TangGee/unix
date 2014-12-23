#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include "ourhdr.h"

/**
 * 客户端unix协议域地址
 * 
 */
#define CLI_PATH   "/var/tmp/"
#define CLI_PERM   S_IRWXU

int 
cli_conn(const char *name)
{
	int fd,len;
	struct sockaddr_un unix_addr;

	if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
		return -1;
	
	memset(&unix_addr,0,sizeof(unix_addr));

	unix_addr.sun_family=AF_UNIX;
	sprintf(unix_addr.sun_path,"%s%05d",CLI_PATH,getpid());

	len=strlen(unix_addr.sun_path)+sizeof(unix_addr.sun_family);

	if(len!=16)
		err_quit("lenth !=16");
	unlink(unix_addr.sun_path);
	if(bind(fd,(struct sockaddr *)&unix_addr,len)<0)
		return -2;
	if(chmod(unix_addr.sun_path,CLI_PERM)<0)
		return -3;

	memset(&unix_addr,0, sizeof(unix_addr));

	unix_addr.sun_family=AF_UNIX;
	strcpy(unix_addr.sun_path,name);


	len=strlen(unix_addr.sun_path)+sizeof(unix_addr.sun_family);

	if(connect(fd,(struct sockaddr *)&unix_addr,len)<0)
		return -4;
	return fd;


}
