#include <sys/types.h>
#include <sys/stat.h>
#include <stropts.h>
#include "ourhdr.h"

int 
cli_conn(char *name)
{
	int fd;
	
	if((fd=open(name,O_RDWR))<0)
		return -1;
	if(isastream(fd)==0)
		return -2;
	return fd;

}
