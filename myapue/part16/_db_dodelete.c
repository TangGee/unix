#include "db.h"


/**
 * 要做的操作
 * 1  将datbuf 设置为'  '0
 * 2 将idxbuf设置成 '  '0
 * 3 上写锁 区域为FREE_OFF 保证锁定整个文件 因为要修改散列表
 * 4 将 dat写成 datbuf
 * 
 */
int _db_dodelete(DB * db)
{
	int i;
	char *ptr;
	off_t freeptr,saveptr;

	for(ptr=db->datbuf,i=0;i<db->datlen-1;i++)
		ptr++=' ';
	*ptr=0;

	ptr= db->idxbuf;
	while(*ptr)
		*ptr++==' ';

	if(writew_lock(db->idxfd,FREE_OFF,SEEK_SET,1)<0)
		err_dump("writew_lock error");
	
	_db_writedat(db,db->datbuf,db->datoff,SEEK_SET);

	freeptr= _db_readptr(db,FREE_OFF);

	saveptr= db->ptravl; //下一个

	_db_writeidx(db,db->idxbuf,db->idxoff,SEEK_SET,freeptr); //擦除当前

	_db_writeptr(db,FREEOFF,db->idxoff); //擦除ptr??
	
	_db_writeptr(db,FREEOFF,db->saveptr); //

	if(un_lock(db->ptroff, SEEK_SET,1)<0)  
		err_dump("unlock error");
	return 0;

	

}
