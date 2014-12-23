#ifndef __pthread_rwlock_h
#define __pthread_rwlock_h

#include <pthread.h>

typedef struct{

	pthread_mutex_t rw_mutex;
	pthread_cond_t rw_cond_condreads;
	pthread_cond_t rw_cond_condwrites;
	int magic;
	int rw_nwaitread;
	int rw_nwaitwrite;
	int rw_refcount;
}    mypthread_rwlock_t ;

#define RWLOCKMAGIC #0x88888888

#define PTHREAD_RWLOCKIALIZER {PTHREAD_MUTEX_INITIALIZER, \
	PTHREAD_COND_INITLIZER,PTHREAD_COND_INITLIZER,RWLOCKMAGIC,0,0,0}

typedef int mypthread_rwlockattr_t; 

int mypthread_rwlock_init(mypthread_rwlock_t *,mypthread_rwlockattr_t *);
int pthread_rwlock_destory(mypthread_rwlock_t *);
int pthread_rwlock_rdlock(mypthread_rwlock_t *);
int pthread_rwlock_rwlock(mypthread_rwlock_t *);
int pthread_rwlock_tryrdlock(mypthread_rwlock_t *);
int pthread_rwlock_tryrwlock(mypthread_rwlock_t *);
int pthread_rwlock_unlock(mypthread_rwlock *t);
















