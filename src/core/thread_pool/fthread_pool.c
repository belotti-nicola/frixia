#include "fthread_pool.h"
#include "../fqueue/frixia_queue.h"
#include <stdlib.h>

thread_pool_t *create_thread_pool(int threads_number, void* (*dispatcher_fun)(void *))
{
    thread_safe_queue_t *q = (thread_safe_queue_t*)malloc(sizeof(thread_safe_queue_t*));
    if (q == NULL)
    {
        return NULL;
    }
    thread_pool_t *t = (thread_pool_t*)malloc(sizeof(thread_pool_t));
    if (t == NULL)
    {
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
}
void thread_pool_add_job(thread_pool_t *t, void* job)
{
    push_q(t->q,job);
}
void thread_pool_join(thread_pool_t *t)
{

}