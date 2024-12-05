#include <stdlib.h>
#include "../fevent/frixia_events_queue.h"
#include "../fevent/frixia_event.h"

#include "frixia_thread_pool.h"

void *thread_main_loop(void *arg)
{
    frixia_events_queue_t *q = (frixia_events_queue_t *)arg;
    while(true)
    {
        frixia_event_t *e = frixia_events_queue_pop(q);
        printf("thread pool!!!%d\n",e->fd);
    }
}

frixia_thread_pool_t* create_frixia_thread_pool(int n)
{
    frixia_thread_pool_t *ptr = malloc(sizeof(frixia_thread_pool_t));
    if(ptr == NULL)
    {
        printf("ERROR CREATING FRIXIA THREAD POOL POINTER\n");
        return NULL;
    }

    ptr->tasks = NULL;


    frixia_thread_pool_t *t = (frixia_thread_pool_t*)malloc(n*sizeof(frixia_thread_pool_t));
    if (t == NULL)
    {
        printf("ERROR CREATING thread_pool_t");
        return NULL;
    }

    pthread_t *threads = (pthread_t*)malloc(n*sizeof(pthread_t));
    frixia_events_queue_t **queues = (frixia_events_queue_t **)malloc(n*sizeof(frixia_events_queue_t *));
    for(int i=0;i<n;i++)
    {
        pthread_t th;
        frixia_events_queue_t *q = frixia_events_queue_create();
        int exit_code = pthread_create(&th,NULL,thread_main_loop,q);
        if(exit_code != 0) { exit(-1);}
        threads[i] = th;
        queues[i] = q;
    }
    ptr->th = threads;
    ptr->threads_tasks = queues;
    return ptr;
}

void set_frixia_thread_pool_tasks(frixia_thread_pool_t *ptr, frixia_events_queue_t *tasks)
{
    ptr->tasks = tasks;
}

