#include <stdbool.h>
#include "frixia_dispatcher.h"

#include "frixia_dispatcher_loop_function.h"

int frixia_dispatcher_loop_function(void *arg)
{
    printf("frixia_dispatcher_loop_function started\n");
    frixia_dispatcher_t   *dispatcher = (frixia_dispatcher_t *)arg;
    frixia_events_queue_t *events_queue = dispatcher->tasks;
    frixia_event_t        *event;
    
    bool keep_looping = true;
    while(keep_looping)
    {
        printf("keep looping dispatcher true\n");
        event = frixia_events_queue_pop(events_queue);
        if(event == NULL)
        {
            printf("ERROR POPPING EVENTS QUEUE\n");
            continue;
        }
        printf("EVENT POPPED BY DISPATCHER::fd::%d\n",event->fd);
    }

    destroy_frixia_dispatcher(dispatcher);
    return 0;
}