#include "pthread_rwlock.h"

int 
pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	int result;

	if(rw->rw_magic!=RWLOCKMAGIC)
		return EINVAL;

	if((result=pthread_mutex_lock(&rw->rw_mutex))!=0)
		return result;


	//如果没有读者 或者有写者在操作 我们就阻塞到这里  ok? :-) 
	//因为我们很优雅的 让写者优先
	//这里当用户拿到写锁后就放弃rw_mutex i
	/*
	 * These functions atomically release mutex and cause the calling thread
	 * to block on the condition variable cond; atomically here means"atomi‐
	 * cally with respect  to access by another thread to the mutex and then
	 * the condition variable".That is,if another thread is able to  acquire
	 * the mutex afterthe about-to-block thread has released it, then a sub‐
	 * sequent call to pthread_cond_broadcast() or pthread_cond_signal() in
	 * that thread shall behave as if it were 
	 * issued after the about-to-block
	 * thread has blocked.
	 */
	while(rw->rw_refcount<0|| rw->rw_nwaitwrite>0){
		rw->rw_nwaitread++;
		result=pthread_cond_wait(&rw->rw_condreads,&rw->rw_mutex);
		rw->rw_nwaitread--;

		if(result!=0)
		{
	            pthread_mutex_unlock(&rw->rw_mutex);
		    return result;
		}


	}


	rw->rw_refcount++;
	pthread_mutex_unlock(&rw->rw_mutex);

	return 0;

}
