#include "../filedescriptor/types/tcp/frixia_tcp.h"
#include "../filedescriptor/types/udp/frixia_udp.h"
#include "../filedescriptor/types/fifo/frixia_fifo.h"
#include "../filedescriptor/fd_monitor/detached_epoll_monitor.c"
#include <stdlib.h>

#include "frixia_environment.h"


//todo
#include "../filedescriptor/fd_monitor/epoll/fepoll_context.h"
void fenv_push_event_from_fepoll(fepoll_ctx_t *ctx)
{
    fepoll_context_push_event(ctx);
}

void fenv_start_tcp_listening(frixia_environment_t *env,const char *ip, int port)
{
    if ( env == NULL )
    {
        return;
    }
    if ( env->filedescriptors >= env->maximum_filedescriptors)
    {
        printf("Error!\n");
        return;
    }

    int rc = start_tcp_listening(ip,port);
    if ( rc <= 0 )
    {
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    frixia_events_queue_t *fepoll_events = env->fepoll_events;
    sv_callback_t *sv = sv_create_callback(fenv_push_event_from_fepoll,NULL);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    env->filedescriptors += 1; 
}
void fenv_start_udp_listening(frixia_environment_t *env,const char *ip, int port)
{
    if ( env == NULL )
    {
        return;
    }
    if ( env->filedescriptors >= env->maximum_filedescriptors)
    {
        printf("Error!\n");
        return;
    }

    int rc = start_udp_listening(port);
    if ( rc <= 0 )
    {
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    sv_callback_t *sv = sv_create_callback(fenv_push_event_from_fepoll,NULL);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    env->filedescriptors += 1;
}

void fenv_start_fifo_listening(frixia_environment_t *env, const char *pipe)
{
    if ( env == NULL )
    {
        return;
    }
    if ( env->filedescriptors >= env->maximum_filedescriptors)
    {
        printf("Error!\n");
        return;
    }

    int rc = start_fifo_listening(pipe);
    if ( rc <= 0 )
    {
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    sv_callback_t *sv = sv_create_callback(fenv_push_event_from_fepoll,NULL);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    env->filedescriptors += 1;
}

frixia_environment_t *fenv_create(int maximum_filedescriptors)
{
    frixia_environment_t *p = malloc(sizeof(frixia_environment_t));
    if ( p == NULL )
    {
        return NULL;
    }

    frixia_epoll_t *fepoll = create_frixia_epoll();
    if ( fepoll == NULL )
    {
        return NULL;
    }
    p->fepoll = fepoll;

    frixia_events_queue_t *q = frixia_events_queue_create();
    if ( q == NULL )
    {
        return NULL;
    }
    p->fepoll_events = q;

    p->maximum_filedescriptors = maximum_filedescriptors;
    p->filedescriptors = 0;
    return p;
}

void fenv_run_engine(frixia_environment_t *fenv)
{
    frixia_epoll_t *fepoll = fenv->fepoll;
    fepoll_th_data_t *fepoll_th = fepoll_th_data_create(fepoll);
    fepoll_th->events = fenv->fepoll_events;
    detached_start_epoll(fepoll_th);


    detached_stop_epoll(fepoll_th);
}

void fenv_destroy(frixia_environment_t *fenv)
{
    destroy_frixia_epoll(fenv->fepoll);
}