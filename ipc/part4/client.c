#include "unpipc.h"


void client(int readfd,int writefd)
{

	size_t len;
	ssize_t n;

	char buff[MAXLINE];

	Fgets(buff,MAXLINE,stdin);
	len=strlen(buff);
	if(buff[len-1]=='\n')
	{
		len--;
	}

	Write(writefd,buff,len);

	while((n=Read(readfd,buff,MAXLINE))>0)
	{
		snprintf(n+buff,sizeof(buff)-n,"nnnnnnnn+%ld",n);
		Write(STDOUT_FILENO,buff,strlen(buff));
	}

}

