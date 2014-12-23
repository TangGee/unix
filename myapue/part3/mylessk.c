#include "apue.h"
#include <fcntl.h>
#include <errmethod.h>

 
/**
 * 用lseek函數可以創建有空洞的文件 
 *
 *不真的佔用那麼大的磁盤塊
 */
char buf1[] ="1234567890";
char buf2[]="ABCDEFGHIJ";

int 
main(void)
{
	int fd;

	if((fd=creat("file.hole",FILE_MODE))==-1)
		err_sys("create file file.hole error");
	if(write(fd,buf1,10)!=10)
		err_sys("write buf1 error");
	if(lseek(fd,1*1024*1024*1024,SEEK_SET)==-1)
		err_sys("lseek error");
	if(write(fd,buf2,10)!=10)
		err_sys("write buf2 error");

	exit(0);
		err_sys("write buf1 error");
}



