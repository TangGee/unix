#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <time.h>

#include "ourhdr.h"

#define STALE 30

int 
serv_accept(int listenfd, uid_t *uidptr)
{
	int clifd,len;
	time_t staletime;
	struct sockaddr_un unix_addr;
	struct stat statbuf;

	len=sizeof(unix_addr);

	if((clifd=accept(listenfd,(struct sockaddr *)&unix_addr,&len))<0)
		return -1;

	len -= sizeof(unix_addr.sun_family);
	unix_addr.sun_path[len]=0;

	if(stat(unix_addr.sun_path,&statbuf)<0)
		return -2;
#ifdef S_ISSOCK
		if(S_ISSOCK(statbuf.st_mode)==0)
			return -3;
#endif

		if((statbuf.st_mode &(S_IRWXG | S_IRWXO))||
				(statbuf.st_mode& S_IRWXU)!= S_IRWXU)
			return -4;
		staletime = time(NULL) - STALE;

		if(statbuf.st_atime<staletime||statbuf.st_ctime<staletime ||
				statbuf.st_mtime < staletime)
			return -5;
		if(uidptr!=NULL)
			*uidptr = statbuf.st_uid;
		unlink(unix_addr.sun_path);

		return (clifd);

}
