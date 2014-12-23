#include "db.h"
#include <sys/uio.h>

/**
 * 要做的操作 
 * 1 db->ptrval 设置成ptrval
 * 2 写db->idxbuf
 * 3 写 下一个chain ptr 和 len
 * 4  上锁
 * 5 写 所有
 * 6 需要解锁则解锁
 */
void db_writeidx(DB *db,const char *key,
		off_t offset,int whence,off_t ptrval)
{
	struct iovec iov[2];
	char asciiptrlen[PTR_SZ+IDXLEN+1];
	int len;

	if((db->ptrval = ptrval)<0 || ptrval> PTR_MAX)
		err_quit("invalid ptr : %d",ptrval);
	sprintf(db->idxbuf,"%s%c%d%c%d\n",key,SEP,db->datoff,SEP,db->datlen);

	if((len=strlen(db->idxbuf))<IDXLEN_MIN|| len>IDXLEN_MAX)
		err_dump("invalid length");
	sprintf(asciiptrlen,"%*d%*d",PTR_SZ,ptrval,IDXLEN_SZ,len);

	if(whence==SEEK_END)
		if(writew_lock(db->idxfd,(db->nhash+1)*PTR_SZ+1,SEEK_SET,0)<0)
			err_dump("write_lock error");

	iov[0].iov_base=asciiptrlen;
	iov[0].iov_len = PTR_SZ+ IDXLEN_SZ;
	iov[1].iov_base = db->idxbuf;
	iov[1].iov_len = len;

	if(writev(db->idxfd,&iov[0],2)!= len+ PTR_SZ_IDXLEN_SZ)
		err_dump("writev error of index record");

	if(whence == SEEK_END)
		if(un_lock(db->idxfd,(db->nhash+1)*PTRSZ+1,0)<0)
			err_dump("un_lock error");

}
