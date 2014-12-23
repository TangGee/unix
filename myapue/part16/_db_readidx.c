#include "db.h"
#include <sys/uio.h>

/**
 *  所要做的操作 
 *  1 设置偏移为offset
 *  2 保存 下一偏移地址到ptrval
 *  3 读取 长度
 *  4 截取 key dataoff  datalen
 *  5 返回 ptrval
 */
off_t 
_db_readidx(DB *db, off_t offset)
{
	int i;
	char *ptr1, *ptr2;
	char asciiptr[PTR_SZ+1], asciilen[IDXLEN_SZ+1];  //6,4
	struct iovec iov[2];

	if((db->idxoff = lseek(db->idxfd,offset,
					offset==0?SEEK_CUR:SEEK_SET))==-1)
		err_dump("lseek error");
	
	iov[0].iov_base = asciiptr;
	iov[0].iov_len = PTR_SZ;
	iov[1].iov_base = asciilen;
	iov[1].iov_len = IDXLEN_SZ;

	if((i=readv(db->idxfd,&iov[0],2))!= PTR_SZ+IDXLEN_SZ){
		if(i==0 && offset==0)
			return -1;
		err_dump("readv error of index record");
	}

	asciiptr[PTR_ZE]=0;
	db->ptrval = atol(asciiptr);

	asciiptr[IDXLEN_ZE]=0;  //6,1024
	if((db->idxlen = atoi(asciilen))< IDXLEN_MIN || db->idxlen> IDXLEN_MAX)
		err_dump("invalid len");
	if((i=read(db->idxfd,db->idxbuf,db->idxlen)!= db->idxlen))
		err_dump("read error of index recode");
	if(db->idxbuf[db->idxlen]!='\n')
		err_dump("missing newline");
	db->idxbuf[db->idxlen-1]=0;

	if((ptr1=strchr(db->idxbuf,SEP))==NULL)
		err_dump("missing first separator");
	
	*ptr1++=0;

	if((ptr2=strchr(ptr1,SEP))==NULL)
		err_dump("missing second separator");
	
	*ptr2++=0;

	if(strchr(ptr2,SEP)!=NULL)
		err_dump("to many separators");

	if((db->datoff = atoi(ptr1))<0)
		err_dump("starting offset <0");
	if((db->datlen = atoi(ptr2))<0)
		err_dump("invalid error");
	return db->ptrval;




}
