#include "pthread_rwlock.h"

int 
pthread_rwlock_init(mypthread_rwlock_t *rw,mypthread_rwlockattr_t *attr )
{
	int result;

	if(attr!=NULL)
		return EINVAL;

	if((result=pthread_mutex_init(rw->mutex))!=0)
		goto err1;
	if((result=pthread_cond_init(rw->rw_cond_condreads))!=0)
		goto err2;
	if((result=pthread_cond_init(rw->rw_cond_condwrites))!=0)
		goto err3;
	rw->magic= RWLOCKMAGIC;
	rw->rw_nwaitread=0;
	rw->rw_nwaitwrite=0;
	rw->rw_refcount=0;

err3:
	pthread_cond_destory(rw->rw_cond_condreads);
err2:
	pthread_mutex_destory(rw->rw_cond_condwrite);
err1:
	return (result);

}
