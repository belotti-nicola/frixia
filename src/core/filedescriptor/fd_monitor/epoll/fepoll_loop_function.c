#include "fepoll.h"
#include "../../../fsuite/frixia_suite.h"
#include "../../../frixia_common.h"
#include "epoll.h"

#include "fepoll_loop_function.h"

int fepoll_loop_function(frixia_suite_t *fsuite)
{
    frixia_epoll_t *fepoll = fsuite->fepoll;

    fadd_stop_filedescriptor(fepoll);

    bool keep_looping = true;
    while(keep_looping)
    {
        frixia_event_t ev_q[FRIXIA_EPOLL_MAXIMUM_EVENTS];
        int events_number = frixia_epoll_wait(fepoll,ev_q);
        printf("events_number :: %d\n",events_number);
        for(int i=0;i<events_number;i++)
        {
            printf("%d\n",ev_q->fd);
            //frixia_events_queue_push();
        }
    }

    stop_fepoll(fepoll);
    return 0;
}