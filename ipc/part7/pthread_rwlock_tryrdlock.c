#include "pthread_rwlock.h"


int 
pthead_rwlock_tryrdlock(pthread_rwlock_t *rw)
{

	      int result;

	      if(rw->rw_magic!=RWLOCKMAGIC)
		      return EINVAL;

	      if((result=pthread_mutex_lock(&rw->rw_mutex))!=0)
		      return result;
	               
	      if(rw->rw_refcount<0|| rw->rw_nwaitwrite>0)
	      {
		      return EBUSY;
	      }else{
		      rw->rw_refcount++;
	      }

		      pthread_mutex_unlock(&rw->rw_mutex);

	      return 0;


}
