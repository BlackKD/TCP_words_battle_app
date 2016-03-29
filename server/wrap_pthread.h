#ifndef __WRAP_PTHREAD_H__
#define __WRAP_PTHREAD_H__

#include <pthread.h>

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func)(void *), void *arg);

void Pthread_exit(void *status);

void Pthread_detach(pthread_t tid);

void Pthread_mutex_lock(pthread_mutex_t *mptr);

void Pthread_mutex_unlock(pthread_mutex_t *mptr);

void Pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);

void Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);

void Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

void Pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

#endif
