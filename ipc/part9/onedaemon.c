#include "unpipc.h"
#include "../lib/wrapstdio.c"
#include "../lib/lock_reg.c"
#include "../lib/wrapunix1.c"

#define PATH_PIDFILE "pidfile"

int main(int argc,char ** argv)
{
	int pidfd;

	char line[MAXLINE];

	pidfd= Open(PATH_PIDFILE, O_RDWR |O_CREAT , FILE_MODE);

	if(write_lock(pidfd,0,SEEK_SET,0)<0){
		if(errno== EACCES || errno==EAGAIN)
		{
			err_quit("unable to lock %s, is %s already running?",

					PATH_PIDFILE,argv[0]);
		}else
			err_sys("unable to lock %s",PATH_PIDFILE);
	}

	snprintf(line,sizeof(line),"%ld\n",(long)getpid());

	Ftruncate(pidfd,0); //截断;

	/**
	 * then do whatever the daemon does ... 
	 */

	pause();
	Write(pidfd,line,strlen(line));

	/**
	 * then do whatever the daemon does ... 
	 */

	pause();
}
