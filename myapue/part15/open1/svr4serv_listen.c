/**
 * 这是系统serv4 下可用的函数 在linux查阅manpage发现是未实现的系统调用
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stropts.h>
#include "ourhdr.h"


/**
 * 将一段压入流管道 就可以使用另一端 读写该流管道
 */
int 
serv_listen(const char * name)
{
	int tempfd,fd[2],len;

	//如果该文件存在删除 
	unlink(name);

	if((tempfd=create(name,FIFO_MODE))<0)
		return -1;
	if(close(tempfd)<0)
		return -2;
	if(pipe(fd))<0
		return -3;

	/**
	 * 将流处理模块压入一端
	 */
	if(ioctl(fd[1],I_PUSH,"connld")<0)
		return -4;

	/**
	 * 使压入的一段具有名称
	 */
	if(fattch(fd[1],name)<0)
		return (-5);

	return fd[0];


}
