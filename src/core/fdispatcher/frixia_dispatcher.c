#include <stdlib.h>

#include "frixia_dispatcher.h"

frixia_dispatcher_t *create_frixia_dispatcher(int workers_number,int stop_fd)
{
    frixia_dispatcher_t *ptr = malloc(sizeof(frixia_dispatcher_t));
    if(ptr == NULL)
    {
        printf("Error in frixia dispatcher creation!\n");
        return NULL;
    }
    ptr->workers = workers_number;
    ptr->stop_fd = stop_fd;
    return ptr;
}

void destroy_frixia_dispatcher(frixia_dispatcher_t *p)
{
    free(p);
}

void set_frixia_dispatcher_tasks(frixia_dispatcher_t *fd, frixia_events_queue_t *fq)
{
    if( fd == NULL || fq == NULL)
    {
        printf("Error in frixia dispatcher set_frixia_dispatcher_tasks!\n");
        return;
    }
    fd->tasks = fq;
}

void set_frixia_dispatcher_thread_pool(frixia_dispatcher_t *fd, frixia_thread_pool_t *tp)
{
    fd->thread_pool = tp;
}

void dispatch_event_to_workers(frixia_dispatcher_t *dispatcher,frixia_event_t *event)
{
    int task_index = dispatcher->task_index;
    int dim = dispatcher->workers;
    frixia_events_queue_t **q = dispatcher->thread_pool->threads_tasks;
    frixia_events_queue_t  *q_i = *(q+task_index);
    frixia_events_queue_push(q_i,event);
    dispatcher->task_index = (task_index + 1)%dim;
}

void dispatch_event_to_all_workers(frixia_dispatcher_t *dispatcher,frixia_event_t *event)
{
    if( dispatcher->thread_pool == NULL )
    {
        printf("Thread pool is null!\n");
        return;
    }

    int dim = dispatcher->workers;
    for(int i=0;i<dim;i++)
    {
        frixia_events_queue_t **q = dispatcher->thread_pool->threads_tasks;
        frixia_events_queue_t  *q_i = *(q+i);
        frixia_events_queue_push(q_i,event);
    }
}