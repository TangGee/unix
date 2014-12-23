#include "apue.h"
#include <errmethod.h>
int 
main(void)
{
	struct stat statbuf;

	if(stat("foo",&statbuf)<0)
		err_sys("stat error for foo");
	if(chmod("foo",(statbuf.st_mode&~S_IXGRP)|S_ISGID)<0)
		err_sys("chmod err for foo");
	if(chmod("bar",S_IRUSR|S_IRGRP|S_IROTH|S_ISGID)<0)
		err_sys("chmod err for bar");

	exit(0);
}
