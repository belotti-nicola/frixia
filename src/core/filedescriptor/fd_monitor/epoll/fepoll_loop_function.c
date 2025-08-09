#include "fepoll.h"
#include "../../../frixia_common.h"
#include "epoll.h"
#include "../../../fevent/frixia_event.h"
#include "../../../filedescriptor/fd_monitor/epoll/fepoll_context.c"
#include <unistd.h>

#include "fepoll_loop_function.h"


void do_callback_wrapper(sv_callback_t *sv, int fd, uint32_t m, frixia_events_queue_t *q, bool *b)
{
    if ( sv == NULL )
    {
        printf("WRN:do_callback NULL!\n");
        return;
    }

    if ( ! sv_is_valid( sv ) )
    {
        printf("WRN:do_callback is not valid!\n");
        return;
    }

    void *(*fun)(void *) = sv->function;
    void *aux            = sv->auxiliary;

    printf("%p %p\n",fun,aux);

    fepoll_ctx_t ctx = 
    {
        .fd = fd,
        .mask = m,
        .events = q,
        .keep_looping = b
    };
    fun(&ctx);
}

int fepoll_loop_function(fepoll_th_data_t *th_data)
{
    frixia_epoll_t *fepoll = th_data->fepoll;

    bool keep_looping = true;
    while( keep_looping )
    {
        printf("Waiting\n");
        frixia_event_t ev_q[FRIXIA_EPOLL_MAXIMUM_EVENTS];
        int events_number = frixia_epoll_wait(fepoll,ev_q);
        printf("events_number :: %d\n",events_number);
        if( events_number < 0)
        {
            //just to not jam up 
            printf("Sleep due to events_number in fepoll < 0\n");
            sleep(1);
            continue;
        }
        for(int i=0;i<events_number;i++)
        {
            printf("event_fd %d(%d events occured), pushing to events_queue\n",ev_q->fd,events_number);
            int event_fd = ev_q->fd;
            uint32_t mask = ev_q->events_maks;
            //printf("FOUND :: %p %p %d!\n",sv.function, sv.auxiliary, sv.is_valid);

            sv_callback_t sv = fepoll->callbacks_data[event_fd];
            do_callback_wrapper(&sv,event_fd,mask,th_data->events,&keep_looping);
        }
    }

    fepoll_stop(fepoll);
    printf("fepoll finished.\n");
    return 0;
}