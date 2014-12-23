#include "unpipc.h"
#include "../lib/wrapsem.c"
#include "../lib/wrapunix1.c"

#define SEM_NAME  "mysem"


int count=0;

int main(int argc,char ** argv)
{
	int i,nloop;

	sem_t *mutex;

	if(argc!=2)
		err_quit("usage : incr1<#loops>");
	nloop =atoi(argv[1]);

	if((mutex = sem_open(SEM_NAME,
			O_CREAT| O_EXCL,FILE_MODE ,1))==SEM_FAILED)
		err_sys("sem_open error");
	Sem_unlink(SEM_NAME);

	setbuf(stdout,NULL);
	if(Fork()==0){
		for(i=0;i<nloop;i++)
		{
			Sem_wait(mutex);
			printf("child: %d\n",count++);
			Sem_post(mutex);
		}
		exit(0);

	}
	for(i=0;i<nloop;i++){
		Sem_wait(mutex);
		printf("parent : %d\n",count++);
		Sem_post(mutex);
	}
	exit(0);
}
