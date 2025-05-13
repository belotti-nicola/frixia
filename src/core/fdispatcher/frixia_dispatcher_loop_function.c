#include <stdbool.h>
#include "frixia_dispatcher_handler.h"
#include "frixia_dispatcher.h"
#include "../callback_suite/callback_data/frixia_callback_context.h"

#include "frixia_dispatcher_loop_function.h"

int frixia_dispatcher_loop_function(void *arg)
{
    printf("frixia_dispatcher_loop_function started\n");
    frixia_dispatcher_t   *dispatcher   = (frixia_dispatcher_t *)arg;
    frixia_events_queue_t *events_queue = dispatcher->tasks;
    bound_robin_t         *bound_robin  = dispatcher->bound_robin; 
    convoy_t              *convoy       = dispatcher->convoy;
    frixia_epoll_t        *fepoll       = dispatcher->fepoll;
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
        int fd = event->fd;
        if( fd == stop_fd )
        {
            printf("Stop event!!!\n");
            *keep_looping = false;
            bound_robin_add_task_to_all_workers(bound_robin,NULL,NULL);
            continue;
        }
        printf("EVENT POPPED BY DISPATCHER::fd::%d\n",fd);

        void *(*fun)(void *) = get_callback_fun(convoy,fepoll,fd);
        void   *arg          = get_callback_arg(convoy,fepoll,fd);
        bound_robin_add_task_to_one_worker(bound_robin,fun,arg);
    }

    printf("frixia_dispatcher_loop_function: END.\n");
    return 0;
}