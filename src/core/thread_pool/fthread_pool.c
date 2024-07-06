#include "fthread_pool.h"
#include "../fqueue/frixia_queue.h"
#include <stdlib.h>
#include <string.h>

thread_pool_t *create_thread_pool(int threads_number, void* (*dispatcher_fun)(void *))
{
    thread_pool_t *t = (thread_pool_t*)malloc(threads_number*sizeof(thread_pool_t));
    if (t == NULL)
    {
        printf("ERROR CREATING thread_pool_t");
        return NULL;
    }
    
    thread_pool_data_t* args = create_thread_pool_data();   
    if (args == NULL)
    {
        printf("ERROR CREATING thread_pool_data_t");
        return NULL;
    }
    t->args = args;



    pthread_t *threads = (pthread_t*)malloc(threads_number*sizeof(pthread_t));
    for(int i=0;i<threads_number;i++)
    {
        pthread_t th;
        int exit_code = pthread_create(&th,NULL,dispatcher_fun,t->args);
        if(exit_code != 0) { exit(EXIT_FAILURE);}
        threads[i] = th;
    }
    t->threads = threads;
    t->threads_number = threads_number;
    return t;
}
void thread_pool_add_job(thread_pool_t *t, void* job)
{
    if( job == NULL )
    {
        printf("Job pointer is null.\n");
        return;
    }
    thread_safe_queue_t *tsq = t->args->q;
    push_q(tsq,job);
}
void thread_pool_join(thread_pool_t *t)
{
    //VERY POOR PRACTICE 
    //MISLEADING (JOIN BUT CANCEL/KILL)
    //DOES NOT CARE OF MUTEXES LOCKS SEMAPHORES
    //TODO
    int dim = t->threads_number;
    for(int i=0;i<dim;i++)
    {
        pthread_t wait_th = t->threads[i];
        int rc = pthread_cancel(wait_th);
        printf("thread %d killed(exit code:%d)\n",i,rc);
    }
}

thread_pool_data_t* create_thread_pool_data()
{
    thread_pool_data_t* args = malloc(sizeof(thread_pool_data_t));
    if(args == NULL)         { return NULL; }
    args->q                  = create_q();

    return args;
}
void destroy_thread_pool_data(thread_pool_data_t* d)
{
    destroy_q(d->q);
    free(d);
}
