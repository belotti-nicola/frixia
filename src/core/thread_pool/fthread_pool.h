#ifndef FTHREAD_POOL_H
#define FTHREAD_POOL_H


#include "../fqueue/frixia_queue.h"


typedef struct thread_pool
{
    int                      size;
    thread_safe_queue_t     *q;
    pthread_t               *threads;
      
} thread_pool_t;


thread_pool_t* create_thread_pool(int n, void *(*dispatcher_fun)(void *));
void thread_pool_add_job(thread_pool_t *t, void* j);
void thread_pool_join(thread_pool_t *t);

#endif