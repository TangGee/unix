#include "db.h"
#include <sys/uio.h>
/**
 * 所左操作
 * 1 需要加锁的加锁
 * 2 写数据
 */
void 
_db_writedat(DB *db, const char *data,off_t offset, int whence)
{
	struct iovec iov[2];
	static char newline = '\n';

	if(whence == SEEK_END)
		if(writew_lock(db->datfd,0,SEEK_SET,0)<0)
			err_dump("writew_lock error");

	if((db->datoff = lseek(db->datfd,offset,whence))==-1)
		err_dump("lseek error");
	db->datlen = strlen(data)+1;
	
	iov[0].iov_base =(char *) data;
	iov[0].iov_len= db->datlen-1;
	iov[1].iov_base = &nawline;
	iov[1].iov_len = 1;

	if(writev(db->datfd,&iov[0],2)!=db->datlen)
		err_dump("writev error of dat record");

	if(whence == SEEK_END)
		if(un_lock(db->datfd,0, SEEK_SET,0)<0)
			err_dump("unlock error");


}

