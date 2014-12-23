#include "unpipc.h"

pid_t 
lock_test(int fd,int type,off_t offset,int whence,off_t len)
{
	struct flock lock;

	lock.l_type = type;
	lock.l_start = offset;
	lock.l_len = len;
	lock.l_whence = whence;

	if(fcntl(fd, F_GETLK, &lock) == -1)
		return -1;
	if(lock.l_type == F_UNLCK)
		return 0;

	return (lock.l_pid);
}

pid_t
Lock_test(int fd,int type,off_t offset,int whence,off_t len)
{
	pid_t pid;

	if((pid=lock_test(fd,type,offset,whence,len))==-1)
		err_sys("lock_test error");

	return pid;
}














