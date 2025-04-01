#include "bound_robin.h"

int bound_robin_add_task(bound_robin_t *br, void *task)
{
    int current_index = br->current_index;

    threadsafe_simple_queue_t *target = br->event_in[current_index];
    push_threadsafe_simple_queue(target,task);

    br->current_index = (current_index + 1)%FRIXIA_WORKERS;
}
int bound_robin_broadcast_task(bound_robin_t *br, void *task)
{
    for( int i=0;i<FRIXIA_WORKERS;i++)
    {
        threadsafe_simple_queue_t *target = br->event_in[i];
        push_threadsafe_simple_queue(target,task);
    }
}