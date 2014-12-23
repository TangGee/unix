#include "ourhdr.h"


int main(void)
{
	int n;
	char buf[MAXLINE];
	while((n=read(STDIN_FILENO,buf,MAXLINE))!=1){
		printf("%d\n",n);
		printf("%s",buf);
	}
}
