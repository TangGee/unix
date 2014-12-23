#include "db.h"
/**
 * 1取出key所在的三列表的位置 加上便宜 求出来 第一条记录的位置
 * 2 设置ptroff 也就是散列表的位置的便宜量
 * 3 枷锁
 * 4 读取三列表中的chain ptr 的值
 * 5 解析记录和下一chain ptr 指向的记录
 * 
 */
int _db_find(DB *db, const char *key,int writelock)
{
	off_t offset,nextoffset;

	db->chainoff = (_db_hash(db,key) 
			*PTR_SZ)+db->hashoff;//取出key散列表的索引位置 然后加上0
	db->ptroff = db-> chainoff; //

	if(writelock){
		if(writew_lock(db->idxfd,db->chainoff,SEEK_SET,1)<0)
			err_dump("writew_lock error");
	}else{
		if(readw_lock(db->idxfd,db->chainoff,SEEK_SET,1)<0)
			err_dump("readw_lock error");
	}

	offset = _db_readptr(db,db->ptroff);//读取三列表项的值

	while(offset!=0){
		nextoffset = _db_readidx(db,offset);//?????
		if(strcmp(key,db->idxbuf)==0)
			break;
		db->ptroff = nextoffset;
	}

	if(offset == 0)
		return -1;

	return 0;
}
