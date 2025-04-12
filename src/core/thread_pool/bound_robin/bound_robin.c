#include <pthread.h>
#include <unistd.h>
#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include <stdlib.h>
#include <stdbool.h>
#include "bound_robin_event.h"

#include "bound_robin.h"


void bound_robin_add_task_to_one_worker(bound_robin_t *br, bound_robin_event_t *task)
{
    int index = br->current_index;
    threadsafe_simple_queue_t *q = br->th_contex[index]->thread_events;
    push_threadsafe_simple_queue(q,(void *)task);
    br->current_index = br->current_index + 1;
}
void bound_robin_add_task_to_all_workers(bound_robin_t *br, bound_robin_event_t *task)
{
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        threadsafe_simple_queue_t *q = br->th_contex[i]->thread_events;
        push_threadsafe_simple_queue(q,(void *)task);
    }
}

void bound_robin_create(bound_robin_t *br)
{
    pthread_barrier_t *barrier = malloc(sizeof(pthread_barrier_t));
    if( barrier == NULL)
    {
        printf("Error pthread barrier!!\n");
        return;
    }
    br->create_barrier = barrier;
    pthread_barrier_init(barrier, NULL, FRIXIA_WORKERS+1);

    br->current_index = 0;

    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        threadsafe_simple_queue_t *q = create_threadsafe_simple_queue();
        thread_context_t *new_context = bound_robin_create_thread_context();
        new_context->thread_events = q;        
        new_context->create_barrier = br->create_barrier;

        void *casted_thread_arg = (void *)new_context;
        int rc = pthread_create(&(br->th[i]),NULL,bound_robin_thread_main_loop,casted_thread_arg);
        if(rc < 0)
        {
            printf("Error pthread_create: %d\n",rc); 
            continue;
        }
        br->th_contex[i] = new_context;
    }


    int rc = pthread_barrier_wait(br->create_barrier);
    printf("RC::%d\n",rc);
}

void bound_robin_wait(bound_robin_t *br)
{
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        thread_context_t *ctx = br->th_contex[i];
        bound_robin_thread_stop(ctx);
    }

    bound_robin_add_task_to_all_workers(br,NULL);
    
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        int rc = pthread_join(br->th[i],NULL);
        if( rc < 0 )
        {
            printf("Error bound robin:: join rc %d\n",rc);
        }
    }
}

void bound_robin_stop(bound_robin_t *br)
{
    thread_context_t *ctx;
    for (int i = 0; i < FRIXIA_WORKERS; i++)
    {
        ctx = br->th_contex[i];
        bound_robin_thread_stop(ctx);
    }
}