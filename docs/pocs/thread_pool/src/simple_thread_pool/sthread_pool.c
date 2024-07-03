#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sthread_pool.h"
#include "../simple_queue/squeue.h"

thread_pool_t *thread_pool_create(int n,void *(*f)(void *), thread_safe_queue_t* input_q)
{
    if( input_q == NULL)
    {
        input_q = create_q();
    }
    
    thread_pool_t* tp = malloc(sizeof(thread_pool_t));
    tp->q = input_q;

    pthread_t th;
    for(int i=0;i<n;i++)
    {
        int exit_code = pthread_create(&th,NULL,f,input_q);
        if(exit_code != 0) { printf("ERRORCODE::%d\n",exit_code);}
        pthread_detach(th);
        printf("Thread started::%d\n",i);
    }

    return tp;
}

void thread_pool_add_job(thread_pool_t *tm, int v)
{
    push_q(tm->q,v);
    return;
}
void thread_pool_join(thread_pool_t *t)
{
    sleep(10);
}

void thread_pool_destroy(thread_pool_t *t)
{
    destroy_q(t->q);
}