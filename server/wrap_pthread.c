// pthread_ wrapper functions 

#include "netprogram.h"
#include "wrap_pthread.h"

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func)(void *), void *arg) {
	int n = pthread_create(tid, attr, func, arg);
	if(n > 0)
		err_sys("pthread_create error\n");
}

void Pthread_exit(void *status) {
	pthread_exit(status);  // does not return to the caller
}

void Pthread_detach(pthread_t tid) {
	int n = pthread_detach(tid);
	if(n > 0)
		err_sys("pthread_detch error\n");

}

void Pthread_mutex_lock(pthread_mutex_t *mptr) {
	int n = pthread_mutex_lock(mptr);
	if(n > 0)
		err_sys("pthread_mutex_lock error\n");
}

void Pthread_mutex_unlock(pthread_mutex_t *mptr) {
	int n = pthread_mutex_unlock(mptr);
	if(n > 0)
		err_sys("pthread_mutex_unlock error\n");
}

void Pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr) {
	int n = pthread_rwlock_init(rwlock, attr);
	if( n > 0 )
		err_sys("pthread_rwlock_init error\n");
}

void Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) {
	int n = pthread_rwlock_rdlock(rwlock);
	if( n > 0 )
		err_sys("pthread_rwlock_rdlock error\n");
}

void Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) {
	int n = pthread_rwlock_wrlock(rwlock);
	if( n > 0 )
		err_sys("pthread_rwlock_rdlock error\n");
}

void Pthread_rwlock_unlock(pthread_rwlock_t *rwlock) {
	int n = pthread_rwlock_unlock(rwlock);
	if( n > 0 )
		err_sys("pthread_rwlock_unlock error\n");
}
