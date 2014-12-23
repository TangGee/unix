#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include "ourhdr.h"

//插入操作
#define DB_INSERT   1
//替换操作
#define DB_REPLACE  2

#define IDXLEN_SZ   4 /*#ascii chars for length of index record*/
#define IDXLEN_MIN  6 /*key,sep,start,sep,length,newline 总长度*/
#define IDXLEN_MAX  1024
#define SEP         ':'
#define DATLEN_MAX  1024
#define DATLEN_MIN  2

#define PTR_SZ      6 
#define PTR_MAX     999999
#define NHASH_DEF   137
#define FREE_OFF    0  //free ptr的位置
#define HASH_OFF    PTR_SZ

typedef struct{
	int idxfd; //索引文件fd
	int datfd; // dat 文件fd
	int oflag; // 打开标志
	
	char *idxbuf; // 索引缓存
	char *datbuf; // 数据缓存
	char *name;   // 文件名
	off_t idxoff;  //本记录项的偏移 这应该和ptroff一样啊 

	size_t idxlen; //idx长度

	off_t datoff;  //数据在数据文件的偏移
	size_t datlen; // 数据的长度
	
	off_t ptrval;   // 保存链中下一指针(当前要寻找的)
	off_t chainoff; //在散列表中的便宜
	off_t ptroff;   // 刚刚遍读取节点的偏移
	off_t hashoff;  // hash 算法的偏移
	
	int nhash;      // hash数  这里是137
	long cnt_delok; //
	long cnt_delerr;
	long cnt_fetchok;    //查找到次数
	long cnt_fetcherr;  //未查找到数 
	long cnt_nextrec;
	long cnt_stor1;
	long cnt_stor2;
	long cnt_stor3;
	long cnt_stor4;
	long cnt_storerr;

} DB;

typedef unsigned long  hash_t;

DB     *db_open(const char *,int, int);
void   db_close(DB *);
char   *db_fentch(DB *,const char *);
int    db_store(DB *, const char *);
int    db_delete(DB *, const char *);
void   db_rewind(DB *);
char   *db_nextrec(DB *,char *);
void   db_stats(DB *);

DB     *_db_alloc(int);
int    _db_checkfree(BD *);
int    _db_dodelete(DB *);
int    _db_emptykey(char *);
int    _db_find(DB *, const char *,int);
int    _db_findfree(DB *);
int    _db_free(BD *);
hash_t _db_hash(DB *,const char * );
char   *_db_nextkey(DB *);
char   *_db_readdat(DB *);
off_t  _db_readidx(DB *,off_t );

off_t  _db_readptr(DB *,off_t);
void   _db_writedat(DB *,const char *,off_t ,int);
void   _db_writeidx(DB *, const char *,off_t ,int ,off_t);
void   _db_writeptr(BD *, off_t , off_t);

