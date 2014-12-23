#include "unpipc.h"
#include "../lib/wrappthread.c"
#include "../lib/wrapsem.c"
#include "../lib/wrapstdio.c"

#define NBUFF 8

struct {
	struct {
		char data[BUFFSIZE];
		ssize_t n;
	}buff[NBUFF];

	sem_t mutex,nempty,nstored;
}shared;

int fd;
void * produce(void *), *consume(void *);

int main(int argc,char **argv)
{

	pthread_t produce_tid,consume_tid; 


	if(argc!=2)
		err_quit("usage: mycat <pathname> ");

	if((fd=open(argv[1],O_RDONLY))==-1)
		err_sys("open error");

	Sem_init(&shared.mutex,0,1);
	Sem_init(&shared.nempty,0,NBUFF);
	Sem_init(&shared.nstored,0,0);


	Set_concurrency(2);

	Pthread_create(&produce_tid,NULL,produce,NULL);
	Pthread_create(&consume_tid,NULL,consume,NULL);

	Pthread_join(produce_tid,NULL);
	Pthread_join(consume_tid,NULL);

	Sem_destroy(&shared.mutex);
	Sem_destroy(&shared.nempty);
	Sem_destroy(&shared.nstored);
	exit(0);
}

void * produce(void *arg)
{
	int i;

	
	//写
	for(i=0;;){
	        Sem_wait(&shared.nempty);
	        Sem_wait(&shared.mutex);

		Sem_post(&shared.mutex);
		shared.buff[i].n=Read(fd,shared.buff[i].data,BUFFSIZE);
		if(shared.buff[i].n==0){
			Sem_post(&shared.nstored);
			return NULL;
		}

		if(++i==NBUFF)
			i=0;
		Sem_post(&shared.nstored);

	}

}
    
void  *consume(void * arg)
{
	int i;

	for(i=0;;){
		Sem_wait(&shared.nstored);
		Sem_wait(&shared.mutex);
		Sem_post(&shared.mutex);
		
		if(shared.buff[i].n==0)
		{
			return NULL;
		}
		Write(STDOUT_FILENO, shared.buff[i].data, shared.buff[i].n);

		if(++i==NBUFF)
			i=0;

		Sem_post(&shared.nempty);

	}



}


