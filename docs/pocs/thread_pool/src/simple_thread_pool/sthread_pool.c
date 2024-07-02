#include <stdio.h>
#include <stdlib.h>

#include "sthread_pool.h"
#include "../simple_queue/squeue.h"

thread_pool_t *thread_pool_create(int n,void *(*f)(void*))
{
    

    thread_pool_t *tp = malloc(sizeof(thread_pool_t));
    
    thread_safe_queue_t *q = create_q();
    tp->q = q;

    pthread_t th;
    for(int i=0;i<n;i++)
    {
        int exit_code = pthread_create(&th,NULL,f,NULL);
        if(exit_code != 0) { printf("ERRORCODE::%d\n",exit_code);}
        pthread_detach(th);
    }

    return NULL;
}


void tpool_add_work(thread_pool_t *tm, void *arg)
{
    int* casted_arg = (int*)arg;
    push_q(tm->q,*casted_arg);
}
void thread_pool_join(thread_pool_t *t)
{

}