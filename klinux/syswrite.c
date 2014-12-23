/*本汇编程序利用系统调用sys_write（）实现显示函数 int mywrite(int fd, char * buf,int cout)。 函数int myadd(int a,int b,int *res) 用于执行a+b=res的晕眩。如果函数返回0 说明溢出  注意 如果在现在linux系统中编译  则去掉函数名钱的下划线“
 */

#include <string.h>
#include <stdio.h>

int main()
{
	char buf[1024];
	int a, b ,res;
	char * mystr= "Calculating...\n";
	char * emsg = "Error in adding\n";

	a=5;b=10;
	mywrite(1,mystr,strlen(mystr));
	if(myadd(a,b,&res))
	{
		sprintf(buf,"The result is %d\n",res);
	}else{
	        mywrite(1,emsg,strlen(emsg));
	}
    return 0;
}
