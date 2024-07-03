#ifndef STHREAD_POOL_H
#define STHREAD_POOL_H

#include <pthread.h>

#include "../simple_queue/squeue.h"

typedef struct thread_pool
{
    pthread_t *th;
    thread_safe_queue_t *q;
      
} thread_pool_t;


thread_pool_t *thread_pool_create(int n,void *(*f)(void *));
void thread_pool_add_job(thread_pool_t *t, void *arg);
void thread_pool_join(thread_pool_t *t);
#endif