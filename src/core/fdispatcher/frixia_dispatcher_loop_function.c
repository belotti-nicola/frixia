#include <stdbool.h>
#include "frixia_dispatcher.h"

#include "frixia_dispatcher_loop_function.h"

int frixia_dispatcher_loop_function(void *arg)
{
    printf("frixia_dispatcher_loop_function started\n");
    frixia_dispatcher_t   *dispatcher   = (frixia_dispatcher_t *)arg;
    frixia_events_queue_t *events_queue = dispatcher->tasks;
    int                    stop_fd      = dispatcher->stop_fd;
    
    frixia_event_t        *event;
    bool *keep_looping = dispatcher->keep_looping;
    while(*keep_looping)
    {
        event = frixia_events_queue_pop(events_queue);
        if(event == NULL)
        {
            printf("ERROR POPPING EVENTS QUEUE\n");
            continue;
        }
        if(event->fd == stop_fd)
        {
            printf("Stop event!!!\n");
            *keep_looping = false;
            dispatch_event_to_all_workers(dispatcher,event);
            continue;
        }
        printf("EVENT POPPED BY DISPATCHER::fd::%d\n",event->fd);
        dispatch_event_to_workers(dispatcher,event);
    }

    printf("frixia_dispatcher_loop_function: END.\n");
    return 0;
}