#include <stdio.h>
#include <errmethod.h>

int main()
{
	char name[L_tmpnam],line[4096];
	FILE *fp;

	printf("tmpfile1 %s \n",tmpnam(NULL));

	tmpnam(name);
	printf("tmpfile2 %s\n",name);

	if((fp=tmpfile())==NULL)
		err_sys("tmpfile error");

	fputs("input a line \n",fp);
	rewind(fp);

	if(fgets(line,sizeof(line),fp)==NULL)
		err_sys("fgets error");
	
	fputs(line,stdout);
}
