#include "pthread_rwlock.h"


int pthread_rwlock_wrlock(pthread_rwlock_t *rw)
{

	        int result;

		if(rw->rw_magic!=RWLOCKMAGIC)
			return EINVAL;

		if((result=pthread_mutex_lock(&rw->rw_mutex))!=0)
			return result;

		while(rw->rw_refcount!=0){
			rw->rw_nwaitwrite++;
			result=pthread_cond_wait(&rw->rw_cond_condwrites,
					&rw->rw_mutex);
			 rw->rw_nwaitwrite--;

			 if(result!=0)
			 {
				 pthread_mutex_unlock(&rw->rw_mutex);
				 return result;
			 }
		}


		rw->rw_refcount=-1;
		pthread_mutex_unlock(&rw->rw_mutex);

		return 0;




}
