/*
 * 1 首先定义数组最大下标  最大线程数
 *
 * 全局变量 操作次数
 * 数组 存放数据
 *
 * 匿名 结构体  put 包含变量 pthread_mutex_t mutex nput下一个要写入的书
 *  nval 下一个要写入的值
 * 初始化 put PTHREAD_MUTEX_INITIALIZER
 *
 * nread :
 * pthread_mutex_t mutex
 * pthread_cond_t cond;
 * int nready;
 * 初始化  pthread_cond_t 要用PTHREAD_COND_INITIALIZER 初始化
 *
 */

#include "unpipc.h"
#include "../lib/wrappthread.c"
#include <errmethod.h>



#define MAXNITEMS 1000000
#define MAXNTHREADS 100


int nitems;
int buff[MAXNITEMS];

struct {
	pthread_mutex_t mutex;
	int nput;
	int nval;
}put ={
	PTHREAD_MUTEX_INITIALIZER

};

struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int nready;
}nread={
	PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER
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

	Set_concurrency(pthreadcount);

	for(i=0;i<pthreadcount;i++)
	{
		count[i]=0;
		Pthread_create(&tid_produce[i],NULL,produce,&count[i]);
	}

	for(i=0;i<pthreadcount;i++)
	{
		Pthread_join(tid_produce[i],NULL);
		printf("count[%d] = %d\n",i,count[i]);
	}


	Pthread_create(&tid_consume,NULL,consume,NULL);
	Pthread_join(tid_consume,NULL);
												
}


void *produce(void *arg)
{
	for(;;){
		Pthread_mutex_lock(&put.mutex);
		if(put.nput>=nitems){
			Pthread_mutex_unlock(&put.mutex);
			return NULL;
		}
		buff[put.nput++]=put.nval++;
		Pthread_mutex_unlock(&put.mutex);

		Pthread_mutex_lock(&nread.mutex);
		//如果消费者在wait状态下(意味着 nread.nready==0 则唤醒之 被唤醒之后还是要等待锁的)
		if(nread.nready==0)
			Pthread_cond_signal(&nread.cond);

		nread.nready++;
		Pthread_mutex_unlock(&nread.mutex);
		(*(int *)arg) +=1;



	}
}
void *consume (void *arg)
{
	int i;
	for(i=0;i<nitems;i++){
		//要先获取锁 然后判断获取信号
		Pthread_mutex_lock(&nread.mutex);
		while(nread.nready==0)
		  Pthread_cond_wait(&nread.cond,&nread.mutex);

		nread.nready--;

		//这里只同步nready  所以操作完nready就尽快释放锁
		Pthread_mutex_unlock(&nread.mutex);


		if(buff[i]!=i)
		{
			printf("buff[%d]!= %d\n", i,buff[i]);
		}


		
	}

	return NULL;
}







