
#include "fepoll.h"
#include "epoll.h"
#include <internal/frixia_event.h>
#include <unistd.h>
#include <frixia/frixia_environment.h>
#include "event_context.h"
#include "fctx.h"

#include "fepoll_loop_function.h"

void do_callback_wrapper(sv_callback_t *sv, int fd, uint32_t m, frixia_environment_t *fenv)
{
    if ( sv == NULL )
    {
        printf("fepoll_loop_function.c::WRN::do_callback NULL!\n");
        return;
    }

    if ( ! sv_is_valid( sv ) )
    {
        printf("fepoll_loop_function.c::WRN::do_callback is not valid!\n");
        return;
    }

    void *(*fun)(void *) = sv->function;
    void *aux            = sv->auxiliary;

    if ( fun == NULL )
    {
        printf("fepoll_loop_function.c::WRN::do_callback fun is null!\n");
        return;
    }

    event_ctx_t ev_ctx = 
    {
        .event = create_event(fd,m)
    };
    fctx_t ctx = 
    {
        .ev_ctx = &ev_ctx,
        .env = fenv,
        .arg = aux
    };
    fun(&ctx);
}

int fepoll_loop_function(fepoll_th_data_t *th_data)
{
    frixia_epoll_t *fepoll = th_data->fepoll;
    frixia_environment_t *fenv = (frixia_environment_t *)th_data->fenv;

    bool *keep_looping = th_data->keep_looping;
    while( *keep_looping )
    {
        printf("Waiting\n");
        frixia_event_t ev_q[50];
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
            int event_fd = ev_q->fd;
            uint32_t mask = ev_q->events_maks;
            frixia_events_queue_t *events = fenv->fepoll_events;

            sv_callback_t sv = fenv->fepoll_ctx->callbacks[event_fd];
            do_callback_wrapper(&sv,event_fd,mask,fenv);
            printf("Event fd in epoll %d...\n",event_fd);
        }
    }

    convoy_t *convoy = fenv->convoy;
    for(int i=0;i<25;i++)
    {
        if ( convoy->filedescriptors[i].type != UNDEFINED )
        {
            int fd = convoy->filedescriptors[i].fd;
            close(fd);
            printf("Closed fd %d\n",fd);
        }
    }

    fepoll_stop(fepoll);
    return 0;
}