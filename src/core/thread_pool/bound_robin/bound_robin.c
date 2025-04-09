#include <pthread.h>
#include <unistd.h>
#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include <stdlib.h>

#include "bound_robin.h"


void bound_robin_add_task(bound_robin_t *br, void *task)
{
    int index = br->current_index;
    threadsafe_simple_queue_t *q = br->th_contex[index]->thread_events;
    push_threadsafe_simple_queue(q,task);
    br->current_index = br->current_index + 1;
}
void bound_robin_broadcast_task(bound_robin_t *br, void *task)
{
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        threadsafe_simple_queue_t *q = br->th_contex[i]->thread_events;
        push_threadsafe_simple_queue(q,task);
    }
}

void bound_robin_create(bound_robin_t *br,
                        void *th_main_fun(void *),void *th_main_arg,
                        void *th_delegate_fun(void *),void *th_delegate_arg)
{
    
    br->current_index = 0;
    br->delegate_argument = th_delegate_arg;

    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        threadsafe_simple_queue_t *q = create_threadsafe_simple_queue();
        thread_context_t *new_context = bound_robin_create_thread_context();
        new_context->thread_events = q;        
        new_context->cb_main = th_main_fun;
        new_context->cb_arg  = th_main_arg;

        void *casted_thread_arg = (void *)new_context;
        int rc = pthread_create(&(br->th[i]),NULL,bound_robin_thread_main_loop,casted_thread_arg);
        if(rc < 0)
        {
            printf("Error pthread_create: %d\n",rc); 
            continue;
        }
        br->th_contex[i] = new_context;
    }
}

void bound_robin_wait(bound_robin_t *br)
{
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        thread_context_t *ctx = br->th_contex[i]->cb_arg;
        bound_robin_thread_stop(ctx);
    }
    
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        pthread_join(br->th[i],NULL);
    }
}