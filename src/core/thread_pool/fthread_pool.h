#ifndef FTHREAD_POOL_H
#define FTHREAD_POOL_H


#include "../fqueue/frixia_queue.h"
#include <stdbool.h>
typedef struct thread_pool_data
{
    thread_safe_queue_t     *q;

} thread_pool_data_t;

typedef struct thread_pool
{
    thread_pool_data_t      *args;
    pthread_t               *threads;
    int                      threads_number;

} thread_pool_t;

thread_pool_data_t* create_thread_pool_data();
void destroy_thread_pool_data(thread_pool_data_t* d);
thread_pool_t* create_thread_pool(int n, void *(*dispatcher_fun)(void *));
void thread_pool_add_job(thread_pool_t *t, void* j);
void thread_pool_join(thread_pool_t *t);

#endif