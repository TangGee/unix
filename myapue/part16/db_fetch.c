#include "db.h"

/**
 * 1 根据key查找记录
 * 2 查找到则  读取date 命中数+1 否则失败书+1
 *
 *
 * 这个查找到的保存在 db 
 */

char * 
db_fetch(DB *db,const char *key)
{
	char *ptr;

	//这里边 有加锁
	if(_db_find(db,key,0)<0){
		ptr = NULL;
		db->cnt_fetcherr ++;
	}else{
		ptr = _db_readdat(db);
		db->cnt_fetchok ++;
	}

	if(un_lock(db->idxfd,db->chainoff,SEEK_SET,1)<0)
		err_dump("un_lock error");
	return ptr;
}
