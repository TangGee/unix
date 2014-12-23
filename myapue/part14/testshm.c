#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "ourhdr.h"


#define ARRAY_SIZE 4000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE (SHM_R | SHM_W) 

char array[ARRAY_SIZE];


int main(void)
{
	int shmid;
	char *ptr,*shmptr;

	printf("array[] from %p to %p\n",&array[0],&array[ARRAY_SIZE]);
	printf("stack arount %p\n",&shmid);

	if((ptr=malloc(MALLOC_SIZE))==NULL)
		err_sys("maloc error");
	printf("malloced from %p to %p\n",ptr,ptr+MALLOC_SIZE);

	if((shmid=shmget(IPC_PRIVATE,SHM_SIZE,SHM_MODE))<0)
		err_sys("shmget error");
	if((shmptr=shmat(shmid,0,0))==(void *)-1)
		err_sys("shmat error");
	printf("shared memory attached from %p to %p\n",shmptr,shmptr+SHM_SIZE);

	if(shmctl(shmid,IPC_RMID,0)<0)
		err_sys("shmctl error");
	
	

	exit(0);

	
}
