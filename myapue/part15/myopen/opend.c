#include "ourhdr.h"
#include "request.c"


/**
 * 要做的操作
 * 1 读取参数
 * 2 处理参数
 */
int main(void)
{

//	int a=1;
//	while(a){};
	int n;
	char buf[MAXLINE];
	while((n=read(STDIN_FILENO,buf,MAXLINE))>0){
		request(buf,n,STDOUT_FILENO);

	}


	if(n<0)
		err_sys("read error");

	exit(0);

}
