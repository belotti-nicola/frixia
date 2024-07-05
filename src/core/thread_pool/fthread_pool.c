#include "fthread_pool.h"
#include "../fqueue/frixia_queue.h"
#include <stdlib.h>

thread_pool_t *create_thread_pool(int threads_number, void* (*dispatcher_fun)(void *))
{
    thread_safe_queue_t *q = create_q();
    if (q == NULL)
    {
        printf("ERROR CREATING thread_safe_queue_t");
        return NULL;
    }
    thread_pool_t *t = (thread_pool_t*)malloc(threads_number*sizeof(thread_pool_t));
    if (t == NULL)
    {
        printf("ERROR CREATING thread_pool_t");
        return NULL;
    }
    t->q = q;


    pthread_t *threads = (pthread_t*)malloc(threads_number*sizeof(pthread_t));
    for(int i=0;i<threads_number;i++)
    {
        pthread_t th;
        int exit_code = pthread_create(&th,NULL,dispatcher_fun,q);
        if(exit_code != 0) { exit(EXIT_FAILURE);}
        pthread_detach(th);
        threads[i] = th;
    }
    t->threads = threads;
    return t;
}
void thread_pool_add_job(thread_pool_t *t, void* job)
{
    if( job == NULL )
    {
        printf("Job pointer is null.\n");
        return;
    }
    thread_safe_queue_t *tsq = t->q;
    push_q(tsq,job);
}
void thread_pool_join(thread_pool_t *t)
{
    int n = t->size;
    for(int i=0;i<t->size;i++)
    {
        pthread_t wait_th = t->threads[i];
        pthread_join(wait_th,NULL);
    }
}