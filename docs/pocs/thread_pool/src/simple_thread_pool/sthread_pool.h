#ifndef STHREAD_POOL_H
#define STHREAD_POOL_H

#include <pthread.h>

typedef struct thread_pool
{
    pthread_t *th;
      
} thread_pool_t;


thread_pool_t *thread_pool_create(int n, void *(*f)(void*), void *arg);

#endif