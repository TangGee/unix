#include<stdio.h>
extern int end,etext,edata;
extern int _etext,_edata,_end;
int main()
{
	printf("&etext=%p,	&edata=%p,	&end=%p\n",&etext,&edata,&end);
	printf("&_etext=%p,	&_edata=%p,	&_end=%p\n",&_etext,&_edata,&end);        return 0;
}
