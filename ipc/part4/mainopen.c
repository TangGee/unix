#include "unpipc.h"
#include "../lib/wrapunix.c"
#include "../lib/wrapstdio.c"

int main(int argc,char ** argv)
{
	int  n;
	FILE *fp;

	char buff[MAXLINE],command[MAXLINE];


	if(Fgets(buff,MAXLINE,stdin)==NULL)
		err_quit("input error");

	n=strlen(buff);

	if(buff[n-1]=='\n')
		buff[n-1]=0;
	snprintf(command,sizeof(command),"cat %s",buff);

	fp=Popen(command,"r");

	while(Fgets(buff,MAXLINE,fp)!=NULL)
		Fputs(buff,stdout);

	Pclose(fp);
	exit(0);

}
