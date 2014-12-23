#include "ourhdr.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


int 
deamon_init (void)
{
	pid_t pid;

	if((pid=fork())<0)
		return -1;
	else if(pid!=0)
		exit(0);

	setsid();
	chdir("/root");

	umask(0);

	return (0);
}
