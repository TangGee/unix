#include "unpipc.h"
#include "../lib/wrapunix1.c"
#include "../lib/wrapsem.c"

#define SEM_NAME "mysem"


int count;

int main(int argc,char **argv)
{
	int nloop,i,fd,*ptr;
	sem_t *mutex;

	if(argc!=2)
		err_quit("usage :incr2  <#loops>");


	nloop=atoi(argv[1]);
	fd=Open("/dev/zero",O_RDWR);
	Write(fd,"0",1);

	ptr = Mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,
			MAP_SHARED,fd,0);
	Close(fd);

	if((mutex=sem_open(SEM_NAME,O_CREAT|O_EXCL,FILE_MODE,1))==SEM_FAILED)
		err_sys("sem_open error");

	Sem_unlink(SEM_NAME);

	setbuf(stdout,NULL);

	if(Fork()==0){
		for(i=0;i<nloop;i++)
		{
			Sem_wait(mutex);
			printf("child : %d\n",(*ptr)++);
			Sem_post(mutex);

		}
		exit(0);
	}

	 for(i=0;i<nloop;i++)
	 {
		 Sem_wait(mutex);
		 printf("parent : %d\n",(*ptr)++);
		 Sem_post(mutex);

	 }
	 exit(0);

}
