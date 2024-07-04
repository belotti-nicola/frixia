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
    thread_pool_t *t = (thread_pool_t*)malloc(sizeof(thread_pool_t));
    if (t == NULL)
    {
        printf("ERROR CREATING thread_pool_t");
        return NULL;
    }

    t->q = q;
    pthread_t th;
    for(int i=0;i<threads_number;i++)
    {
        int exit_code = pthread_create(&th,NULL,dispatcher_fun,q);
        if(exit_code != 0) { exit(EXIT_FAILURE);}
        pthread_detach(th);
    }
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
    
}