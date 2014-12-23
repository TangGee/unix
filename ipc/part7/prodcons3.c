#include "unpipc.h"
#include "../lib/wrappthread.c"
#include <errmethod.h>


#define MAXNITEMS 1000000
#define MAXNTHREADS 100


int nitems;

struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;//下一次存放的下标
	int nval;//下一次存放的值
}shared={
	PTHREAD_MUTEX_INITIALIZER
};

void *produce(void *);
void *consume (void *);

int main(int argc, char ** argv)
{
	int pthreadcount,i;
	int count[MAXNTHREADS];
	pthread_t tid_consume,tid_produce[MAXNTHREADS];

	if(argc!=3)
		err_quit("usage : <ntimes> <threadcount>");

	nitems=min(atoi(argv[1]),MAXNITEMS);
	pthreadcount=min(atoi(argv[2]),MAXNTHREADS);

	Set_concurrency(pthreadcount+1);

	for(i=0;i<pthreadcount;i++)
	{
		count[i]=0;
		Pthread_create(&tid_produce[i],NULL,produce,&count[i]);
	}

	Pthread_create(&tid_consume,NULL,consume,NULL);

	for(i=0;i<pthreadcount;i++)
	{
		Pthread_join(tid_produce[i],NULL);
		printf("count[%d] = %d\n",i,count[i]);
	}


	Pthread_join(tid_consume,NULL);

}

void *produce (void *arg)
{
	for(;;){
		Pthread_mutex_lock(&shared.mutex);
		if(shared.nput>=nitems){
			 Pthread_mutex_unlock(&shared.mutex);
			 return NULL;
		}

		shared.buff[shared.nput++]=shared.nval++;
		 Pthread_mutex_unlock(&shared.mutex);
		 (*((int *)arg))+=1;


	}
}

void consume_wait(int i)
{
	for(;;){
		Pthread_mutex_lock(&shared.mutex);
		if(i<shared.nput){
			Pthread_mutex_unlock(&shared.mutex);
			return;
		}
		Pthread_mutex_unlock(&shared.mutex);


	}

}

void *consume(void *arg)
{
	int i;
	for(i=0;i<nitems;i++)
        consume_wait(i);
		if(shared.buff[i]!=i)
			printf("buff[%d] = %d\n",i,shared.buff[i]);

	return NULL;
}




