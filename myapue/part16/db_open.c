#include "db.h"

/**
 * 所做的操作  很见简单
 * 1 根据pathname 分配db数据结构所需的空间
 * 2 设置oflag标志
 * 3 设置name
 * 4 打开idx 和 dat文件 
 * 5 如果新建或截断  上锁 
 * 5.5 如果文件的size为0  就填充hash结构 三个空格一个0
 * 6 解锁
 * 7 初始化nhash
 * 8 初始化hashoff
 */

DB *
db_open(const char *pathname,int oflag,int mode)
{
	DB *db;
	int len;
	char asciiptr[PTR_SZ+1],  //6
	hash[(NHASH_DEF+1) * PTR_SZ + 2];//NHASH_DEF 137  +2是回车和NULL
	struct stat statbuff;

	len=strlen(pathname);
	if(db=_db_alloc(len)==NULL)
		err_dump("_db_alloc error for DB");

	db->oflag= oflag;

	/**open index file*/
	strcpy(db->name,pathname); //pathname.idx
	strcat(db->name,".idx");
	if((db->idxfd=(open(db->name,oflag,mode)))<0){
		_db_free(db);
		return (NULL);
	}

	strcat(db->name+len,".dat"); //pathname.dat
	if((db->datfd=(open(db->name,oflag,mod)))<0){
		_db_free(db);
		return (NULL);
	}

	if(oflag & (O_CREAT| O_TRUNC)==(O_CREATE|O_TRUCN)){
		
		if(writew_lock(db->idxfd,0,SEEK_SET,0)<0)
			err_dump("write lock error");
		if(fstat(db->idxfd,&statbuff)<0)
			err_sys("fstat error");
		if(statbuff.st_size==0){
			sprintf(asciiptr,"%*d",PTR_SZ,0);//'    ''0'
			hash[0]=0;
			//NHASH_DEF=137  //加一 是因为 有一个free
			for(i=0;i< (NHASH_DEF +1);i++)
				strcat(hash,asciiptr);

			strcat(hash,"\n");
			i=strlen(hash);
			if(write(db->idxfd,hash,i)!=i)
				err_dump("write error initialilizing 
						index file");
		
		}

		if(un_lock(db->idxfd,0, SEEK_SET,0)<0)
			err_dump("un_lock error");
	}

	db->nhash = NHASH_DEF;
	db->hashoff = HASH_OFF;

	db_rewind(db);

	return db;

}
