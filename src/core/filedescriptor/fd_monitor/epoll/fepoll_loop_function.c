#include "fepoll.h"
#include "../../../fsuite/frixia_suite.h"
#include "../../../frixia_common.h"
#include "epoll.h"
#include "../../../fevent/frixia_event.h"

#include "fepoll_loop_function.h"

int fepoll_loop_function(frixia_suite_t *fsuite)
{
    frixia_epoll_t *fepoll = fsuite->fepoll;

    int stop_fd = fsuite->fepoll->stop_fd;
    bool keep_looping = true;
    while(keep_looping)
    {
        frixia_event_t ev_q[FRIXIA_EPOLL_MAXIMUM_EVENTS];
        int events_number = frixia_epoll_wait(fepoll,ev_q);
        printf("events_number :: %d\n",events_number);
        for(int i=0;i<events_number;i++)
        {
            printf("event_fd %d(%d events occured), pushing to events_queue\n",ev_q->fd,events_number);
            int event_fd = ev_q->fd;
            if( event_fd == stop_fd)
            {
                //possible mapping of callbacks
                //perform some operations based on FDs type 
                keep_looping = false;
                continue;
            }
            frixia_event_t *e = create_event(event_fd);
            frixia_events_queue_push(fsuite->events_q,e);
        }
    }

    printf("fsuite->events_q->dim %d\n",fsuite->events_q->queue->size);
    stop_fepoll(fepoll);
    return 0;
}