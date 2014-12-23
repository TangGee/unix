#include "unpipc.h"
#include <errmethod.h>


void
Sem_close(sem_t *sem)
{
	if (sem_close(sem) == -1)
		err_sys("sem_close error");
}


void
Sem_unlink(const char *pathname)
{
	if (sem_unlink(pathname) == -1)
		err_sys("sem_unlink error");
}

void
Sem_getvalue(sem_t *sem, int *valp)
{
	if (sem_getvalue(sem, valp) == -1)
		err_sys("sem_getvalue error");
}


void
Sem_wait(sem_t *sem)
{
	if (sem_wait(sem) == -1)
		err_sys("sem_wait error");
}



void
Sem_post(sem_t *sem)
{
	if (sem_post(sem) == -1)
		err_sys("sem_post error");
}




void
Sem_init(sem_t *sem, int pshared, unsigned int value)
{
	if (sem_init(sem, pshared, value) == -1)
		err_sys("sem_init error");
}

void
Sem_destroy(sem_t *sem)
{
	if (sem_destroy(sem) == -1)
		err_sys("sem_destroy error");
}

