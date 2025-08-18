#include "../filedescriptor/types/tcp/frixia_tcp.h"
#include "../filedescriptor/types/udp/frixia_udp.h"
#include "../filedescriptor/types/fifo/frixia_fifo.h"
#include "../filedescriptor/fd_monitor/detached_epoll_monitor.c"
#include "../fdispatcher/frixia_dispatcher.h"
#include "../fdispatcher/detached_frixia_dispatcher.h"
#include <stdlib.h>

#include "frixia_environment.h"


//todo
#include "../filedescriptor/fd_monitor/epoll/fepoll_context.h"
void fenv_push_event_from_fepoll(fepoll_ctx_t *ctx)
{
    fepoll_context_push_event(ctx);
}

void fenv_stop_event(fepoll_ctx_t *ctx)
{
    fepoll_context_stop(ctx);
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
    sv_callback_t *sv = sv_create_callback(fenv_push_event_from_fepoll,NULL);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);

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
        printf("Error! %d\n",rc);
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    sv_callback_t *sv = sv_create_callback(fenv_push_event_from_fepoll,NULL);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
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
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;
}

void fenv_set_custom_tcp_callback(frixia_environment_t *env,const char *ip, int port, void *(*fun)(void *), void *arg)
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
    sv_callback_t *sv = sv_create_callback(fun,arg);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;
}

void fenv_set_custom_udp_callback(frixia_environment_t *env,const char *ip, int port, void *(*fun)(void *), void *arg)
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

    int rc = start_udp_listening(ip);
    if ( rc <= 0 )
    {
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    sv_callback_t *sv = sv_create_callback(fun,arg);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;
}
void fenv_set_custom_fifo_callback(frixia_environment_t *env,const char *path, void *(*fun)(void *), void *arg)
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

    int rc = start_fifo_listening(path);
    if ( rc <= 0 )
    {
        return;
    }

    frixia_epoll_t *fepoll = env->fepoll;
    sv_callback_t *sv = sv_create_callback(fun,arg);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
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

    frixia_dispatcher_t *fdispatcher = create_frixia_dispatcher(1,-1);
    if ( fdispatcher == NULL )
    {
        return NULL;
    }
    p->fdispatcher = fdispatcher;

    p->maximum_filedescriptors = maximum_filedescriptors;
    p->filedescriptors = 0;
    return p;
}

void fenv_run_engine(frixia_environment_t *fenv)
{
    frixia_epoll_t *fepoll = fenv->fepoll;
    frixia_dispatcher_t *dispatcher = fenv->fdispatcher;

    fepoll_th_data_t *fepoll_th = fepoll_th_data_create(fepoll);
    fepoll_th->events = fenv->fepoll_events;
    detached_start_epoll(fepoll_th);
    
    dispatcher->fepoll = fepoll;
    dispatcher->tasks = fenv->fepoll_events;
    waitable_frixia_dispatcher_t dispatcher_data;
    dispatcher_data.dispatcher = dispatcher;
    detached_start_frixia_dispatcher(&dispatcher_data);

    detached_join_epoll(fepoll_th);
    detached_stop_frixia_dispatcher(fepoll_th);
}

void fenv_destroy(frixia_environment_t *fenv)
{
    destroy_frixia_epoll(fenv->fepoll);
}