// pthread_ wrapper functions 

#include "netprogram.h"
#include "wrap_thread.h"

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func)(void *), void *arg) {
	int n = pthread_create(tid, attr, func, arg);
	if(n > 0)
		err_sys("pthread_create error\n");
}

void Pthread_exit(void *status) {
	pthread_exit(status);  // does not return to the caller
}

void Pthread_detch(pthread_t tid) {
	int n = pthread_detch(tid);
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

