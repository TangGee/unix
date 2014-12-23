#include "unpipc.h"


int
set_concurrency(int level)
{
#ifdef  HAVE_THR_SETCONCURRENCY_PROTO
	        int             thr_setconcurrency(int);

		        return(thr_setconcurrency(level));
#else
			        return(0);
#endif
}

void
Set_concurrency(int level)
{
	if (set_concurrency(level) != 0)
		err_sys("set_concurrency error");
}

void
Pthread_create(pthread_t *tid, const pthread_attr_t *attr,
		                           void * (*func)(void *), void *arg)
{
	int             n;

	if ( (n = pthread_create(tid, attr, func, arg)) == 0)
		return;
	errno = n;
	err_sys("pthread_create error");
}



void
Pthread_join(pthread_t tid, void **status)
{
	int             n;

	if ( (n = pthread_join(tid, status)) == 0)
		return;
	errno = n;
	err_sys("pthread_join error");
}

void
Pthread_mutex_lock(pthread_mutex_t *mptr)
{
	int             n;

	if ( (n = pthread_mutex_lock(mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_lock error");
}

void
Pthread_mutex_unlock(pthread_mutex_t *mptr)
{
	int             n;

	if ( (n = pthread_mutex_unlock(mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_mutex_unlock error");
}

void
Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr)
{
	int             n;

	if ( (n = pthread_cond_wait(cptr, mptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_wait error");
}

void
Pthread_cond_signal(pthread_cond_t *cptr)
{
	int             n;

	if ( (n = pthread_cond_signal(cptr)) == 0)
		return;
	errno = n;
	err_sys("pthread_cond_signal error");
}



