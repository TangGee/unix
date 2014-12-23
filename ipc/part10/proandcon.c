#include "unpipc.c"
#include "../lib/wrapusem.c"


#define MAXITEMS 1000000
#define MAXTHREAD 100
#define PROSEM "prosemfile"
#define CONSEM "consemfile"

int nthread;
int nitem;

void *produce(void *);
void *consume(void *);

struct {
	pthread_mutex_t mutex;
	sem_t *pro_sem;//生产者要等待的信号量
	pthread_cond_t con_sem; //消费者要等待的信号量
	int nput; //将要添加的数 
	int nget; //将要修取出的数
	int buff[100];  //放数的数组哦
} shared {
	PTHREAD_MUTEX_INITIALIZER,
	NULL,NULL,0,0

};

int 
main(int argc ,char ** argv)
{
	int i,count[MAXTHREAD];
	
	pthread_t propids[MAXTHREAD],conpids[5];
	

	if((shared.pro_sem=(PROSEM,
	sem_open(O_RDWR|O_CREAT,FILE_MODE,0)))==SEM_FAILED)
		goto err1;
	if((shared.con_sem=sem_open(CONSEM,
		O_RDWR|O_CREAT,FILE_MODE,0))==SEM_FAILED)
		goto err2;

	nthread=min(atoi(argv[1]),MAXTHREAD);
	nitem=min(atoi(argv[2]));

	for(i=0;i<nthread;i++){
		count[i]=0;
		pthread_create(&propids[i],NULL,produce,&count[i]);

	}

	for(i=0;i<5;i++)
	{
		pthread_create(&conpids[i],NULL,consume,NULL);
	}



       	for(i=0;i<nthread;i++){
		pthread_join(propids[i],NULL);
		if(i<5)
			pthread_join(propids[i],NULL);

	  }






err2:
	Sem_unlink(shared.pro_sem);
err1:
	err_quit("sem_open error");


}


void *produce(void *arg)
{
	for(;;){
		pthread_mutex_lock(&shared.mutex);
		if(shared.nput)

		pthread_mutex_lock(&shared.mutex);
	}
}
void *consume(void *arg)












