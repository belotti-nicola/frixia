#include "fepoll.h"
#include "../../../frixia_common.h"
#include "epoll.h"
#include "../../../fevent/frixia_event.h"

#include "fepoll_loop_function.h"

int fepoll_loop_function(fepoll_th_data_t *th_data)
{
    frixia_epoll_t *fepoll = th_data->fepoll;

    int stop_fd = fepoll->stop_fd;
    bool keep_looping = true;
    while(keep_looping)
    {
        frixia_event_t ev_q[FRIXIA_EPOLL_MAXIMUM_EVENTS];
        int events_number = frixia_epoll_wait(fepoll,ev_q);
        printf("events_number :: %d\n",events_number);
        if( events_number < 0)
        {
            //just to not jam up 
            wait(1);
        }
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
            frixia_events_queue_push(th_data->events,e);
        }
    }

    stop_fepoll(fepoll);
    return 0;
}