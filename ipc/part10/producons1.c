#include "unpipc.h"
#include "../lib/wrapsem.c"
#include "../lib/wrappthread.c" 


#define NBUFF  10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"

int nitems;

struct {
	int buff[NBUFF];
	sem_t *mutex,*nempty,*nstored;
}shared;

void * produce(void *);
void * consume(void *);

int 
main(int argc,char **argv)
{
	pthread_t protid,contid;
	nitems=atoi(argv[1]);
	if((shared.mutex=sem_open(SEM_MUTEX,
		O_CREAT|O_EXCL,FILE_MODE,1))==SEM_FAILED)
		goto err1;
	if((shared.nempty=sem_open(SEM_NEMPTY,				                     O_CREAT|O_EXCL,FILE_MODE,0))==SEM_FAILED)
		goto err2;
	if((shared.nstored=sem_open(SEM_NSTORED,                                               O_CREAT|O_EXCL,FILE_MODE,0))==SEM_FAILED)
	        goto err3;

	Set_concurrency(2);
	Pthread_create(&protid,NULL, produce,NULL);
	Pthread_create(&contid,NULL,consume,NULL);

	Pthread_join(protid,NULL);
	Pthread_join(contid,NULL);

	Sem_unlink(SEM_MUTEX);
	Sem_unlink(SEM_NEMPTY);
	Sem_unlink(SEM_NSTORED);

	exit(0);




err3:
	Sem_unlink(SEM_NEMPTY);
err2:
	Sem_unlink(SEM_MUTEX);
err1:
	err_quit("sem_open error");


}


void * produce(void *arg)
{

	int i;
	for(i=0;i<nitems;i++)
	{
		Sem_wait(shared.nempty);
		Sem_wait(shared.mutex);
		shared.buff[i%NBUFF]==i;
		Sem_post(shared.mutex);
		Sem_post(shared.nstored);
	}
	return NULL;
}


void * consume(void *arg)
{
	int i;
	for(i=0;i<nitems;i++)
	{
		Sem_wait(shared.nstored);
		Sem_wait(shared.mutex);
		if(shared.buff[i%NBUFF]!=i)
			printf("buff[%d] = %d\n",i,shared.buff[i%NBUFF]);
		Sem_post(shared.mutex);
		Sem_post(shared.nempty);
	}

	return NULL;
}
