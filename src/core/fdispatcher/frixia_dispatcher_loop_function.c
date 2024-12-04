#include <stdbool.h>
#include "frixia_dispatcher.h"

#include "frixia_dispatcher_loop_function.h"

int frixia_dispatcher_loop_function(frixia_dispatcher_t *dispatcher)
{
    frixia_events_queue_t *events_queue = dispatcher->tasks;
    frixia_event_t        *event;
    
    bool keep_looping = true;
    while(true)
    {
        event = frixia_events_queue_pop(events_queue);
        printf("EVENT POPPED BY DISPATCHER::%d\n",event->fd);
    }

    destroy_frixia_dispatcher(dispatcher);
    return 0;
}