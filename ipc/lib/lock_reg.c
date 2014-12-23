#include "unpipc.h"


int lock_reg(int fd, int cmd,int type,off_t offset,int whence,off_t len)
{
	struct flock lock;
	lock.l_type= type;
	lock.l_start= offset;
	lock.l_whence =whence;
	lock.l_len= len;

	return (fcntl(fd,cmd,&lock));
}


void Lock_reg(int fd, int cmd,int type,off_t offset,int whence,off_t len)
{
        
	if(lock_reg(fd, cmd,type, offset,whence,len)==-1)
		err_sys("lock_reg error");
}

