#include <sys/type.h>
#include <fcntl.h>
#include "ourhdr.h"

int 
lock_reg(int fd,int cmd,short type, off_t start,short whence,off_t len)
{
	struct lock;

	lock.l_type=type;
	lock.l_start=start;
	lock.l_whence=whence;
	lock.l_len=len;

	return (fcntl(fd,cmd,&lock));
}


pid_t
lock_test(int fd,int type,off_t start,short whence,off_t len)
{
	struct lock;

	lock.l_type=type;
	lock.l_start=start;
	lcok.l_whence=whence;
	lock.l_len=len;


	if(fcntl(fd,F_GETLK,&lock)<0)
		err_sys("fcntl error");

	if(lock.l_type==F_UNLCK)
		return 0;
	else
		return (lock.l_pid);


}
