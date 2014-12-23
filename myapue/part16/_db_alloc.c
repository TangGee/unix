#include "db.h"

/**
 * 1分配db内存
 * 2 初始化idxfd 和datfd =-1
 * 3 分配name idxbuf datbuf 
 *
 */
DB * 
_db_alloc(int namelen)
{
	DB *db;

	if((db= calloc(1,sizeof(DB)))==NULL)
		err_dump("calloc error for DB");
	db->idxfd = db->datfd=-1;

	if((db->name =malloc(namelen+5))==NULL)
		err_dump("malloc error for name");

	if((db->idxbuf=malloc(IDXLEN_MAX+2))==NULL) // 1024 索引的缓存
		err_dupm("malloc error for idxbuf");
	
	if((db->datbuf=malloc(DATLEN_MAX+2))==NULL) // 1024 索引的缓存
		err_dupm("malloc error for datbuf");//1024+2
	return db;
}
