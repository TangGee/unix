#include "unpipc.h"
#include "../lib/wrapsem.c"

int 
main(int argc ,char **argv)
{
	sem_t *sem;
	int value;

	if(argc!=2)
		err_quit("usage: semgetvalue <name>");


	if((sem=sem_open(argv[1],0))==SEM_FAILED)
		err_sys("sem_open error");

	Sem_getvalue(sem,&value);

	printf("value = %d\n",value);

	exit(0);

}
