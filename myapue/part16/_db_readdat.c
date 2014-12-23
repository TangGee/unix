#include "db.h"

/**
 * 要做的操作 
 * 1 设置文件便宜
 * 2 读取数据
 * 3 将文件'\n' 替换成NULL
 * 4 返回dabuf
 */
char *
_db_readdat(DB *db)
{
	if(lseek(db->datfd,db->datoff,SEEK_SET)==-1)
		err_dump("lseek error");

	if(read(db->datfd,db->datbuf,db->datlen)!=db->datlen)
		err_dump("read error for dat");
	if(db->datbuf[db->datlen-1]!='\n')
		err_dump("missing newline");
	db->datbuf[db->datlen-1] =0;

	return db->datbuf;


}
