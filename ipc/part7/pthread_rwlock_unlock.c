#include "pthread_rwlock.h"
#include "errormethod.h"
int 
pthread_rwlock_unlock(pthread_rwlock_t *rw)
{

	  int result;

	  if(rw->rw_magic!=RWLOCKMAGIC)
		  return EINVAL;

	  if((result=pthread_mutex_lock(&rw->rw_mutex))!=0)
		  return result;

	  if(rw->rw_refcount!=0 || rw->rw_nwaitread>0 || rw->rw_nwaitwrite>0)
		  return EBUSY;

	 

	  if(rw->rw_refcount==-1)
	  {
		  rw->rw_refcount=0;
	  }else if(rw_refcount>0)
		  rw->rw_refcount--;

	  else
		  err_dump("rw_refcount = %d\n",rw->rw_refcount);
	  if(rw->rw_nwaitwrite>0){
		  if(rw->refcount==0)
		       pthread_cond_signal(&rw->rw_cond_condwrites);
	  }
	  else if(rw->rw_nwaitread>0)
		  pthread_cond_broadcast(&rw_cond_condreads);
		  


	  pthread_mutex_lock(&rw->rw_mutex);
	  return 0;

}
