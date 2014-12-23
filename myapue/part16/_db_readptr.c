#include "db.h"

/**
 * 1 lseek找到offset位置
 * 2读PTR_SZ 个字节到ptr
 * 3转换为证书返回
 */
off_t 
_db_readptr(DB * db,off_t offset)
{
	char asciiptr[PTR_SZ+1]; //7

	if(lseek(db->idxfd, offset, SEEK_SET) == -1)
		err_dump("lseek error ptr field");
	if(read(db->idxfd,asciiptr,PTR_SZ)!=PTRSZ)
		err_dump("read error of ptr field");

	asciiptr[PTR_SZ] =0;

	return (atol(asciiptr));

}
