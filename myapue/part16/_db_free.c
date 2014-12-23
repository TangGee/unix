#include "db.h"
/**
 * 要做的操作
 *
 * 1 关闭idxfd 和datfd
 * 2 free idxbuf datbuf name
 *
 */
int 
_db_free(DB *db)
{
	//如果数据库还在打开状态 就关闭它 关闭失败则退出 
	if(db->idxfd > =0 && close(db-> idxfd) <0)
		err_dump("index close error");
	if(db->datfd >=0 && close(db->datfd) <0)
		err_dump("dat close error");
	db->idxfd = db->datfd = -1;
	
	if(db->idxbuf !=NULL)
		free(db->idxbuf);
	if(db->datbuf!=NULL)
		free(db->idxbuf);
	if(db->name!=NULL)
		free(db->name);

	free(db);

	return (0);

}





