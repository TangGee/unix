#include "pthread_rwlock.h"

int pthread_rwlock_destory(pthread_rwlock_t *rw)
{
	

	int avadestory=0;
	int result;

	if(rw->rw_magic!=RWLOCKMAGIC)
		return EINVAL;

	if((result=pthread_mutex_lock(&rw->mutex))!=0)
		return result;

	if(rw->rw_refcount!=0||rw->nwaitread!=0||rw->rw_nwaitwrite!=0)
		return EBUSY;
        avadestory=1;
        pthread_cond_destory(&rw->cond_condreads);
	pthread_cond_destory(&rw->cond_conwrites);
		

	pthread_mutex_unlock(&rw->rw_mutex);

	if(avadestory)
		pthread_mutex_destory(&rw->mutex);

	return result;

}
