#include "db.h"

/**
 * 1  找到该记录 并删除 
 */
int 
db_delete(DB * db,const char *key)
{
	int rc;

	if(_db_find(db,key,1)==0)
	{
		rc= _db_dodelete(db);
		db->cnt_delok++;

	}else{
		rc=-1;
		db->cnt_delerr++;
	}

	if(unlock(db->idxfd,db->chainoff,SEEK_SET,1)<0)
		err_dump("unlock error");
	return rc;
}
