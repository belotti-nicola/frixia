#include "../filedescriptor/types/tcp/frixia_tcp.h"
#include "../filedescriptor/types/udp/frixia_udp.h"
#include "../filedescriptor/types/fifo/frixia_fifo.h"
#include "../filedescriptor/fd_monitor/detached_epoll_monitor.c"
#include "../fdispatcher/frixia_dispatcher.h"
#include "../fdispatcher/detached_frixia_dispatcher.h"
#include <stdlib.h>

#include "frixia_environment.h"


//todo
#include "../fcontexts/fctx.h"

void *fenv_push_event_from_fepoll(void *arg)
{
    fctx_t *ctx = (fctx_t *)arg;
    frixia_events_queue_t *q = ctx->env->fepoll_events;
    frixia_event_t *e = ctx->ev_ctx->event;
    frixia_events_queue_push(q,e);
    return NULL;
}

void *fenv_stop_event(void *arg)
{
    fctx_t *ctx = (fctx_t *)arg;
    //fepoll_context_stop(ctx);
    return NULL;
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

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
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

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
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

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    sv_callback_t *sv = sv_create_callback(fenv_push_event_from_fepoll,NULL);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;
}

void *fenv_set_custom_tcp_callback(frixia_environment_t *env,const char *ip, int port, void *(*fun)(void *), void *arg)
{
    if ( env == NULL )
    {
        return NULL;
    }
    if ( env->filedescriptors >= env->maximum_filedescriptors)
    {
        printf("Error!\n");
        return NULL;
    }

    int rc = start_tcp_listening(ip,port);
    if ( rc <= 0 )
    {
        return NULL;
    }

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    sv_callback_t *sv = sv_create_callback(fun,arg);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;

    return NULL;
}

void *fenv_set_custom_udp_callback(frixia_environment_t *env,const char *ip, int port, void *(*fun)(void *), void *arg)
{
    if ( env == NULL )
    {
        return NULL;
    }
    if ( env->filedescriptors >= env->maximum_filedescriptors)
    {
        printf("Error!\n");
        return NULL;
    }

    int rc = start_udp_listening(port);
    if ( rc <= 0 )
    {
        return NULL;
    }

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    sv_callback_t *sv = sv_create_callback(fun,arg);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;

    return NULL;
}
void *fenv_set_custom_fifo_callback(frixia_environment_t *env,const char *path, void *(*fun)(void *), void *arg)
{
    if ( env == NULL )
    {
        return NULL;
    }
    if ( env->filedescriptors >= env->maximum_filedescriptors)
    {
        printf("Error!\n");
        return NULL;
    }

    int rc = start_fifo_listening(path);
    if ( rc <= 0 )
    {
        return NULL;
    }

    frixia_epoll_t *fepoll = env->fepoll_ctx->fepoll;
    sv_callback_t *sv = sv_create_callback(fun,arg);
    fepoll->callbacks_data[rc] = *sv;

    insert_event(fepoll->fd,rc);
    fepoll_pool_t *fpool = fepoll->fd_pool;
    fepoll_pool_add_fd(fpool,rc);
    env->filedescriptors += 1;

    return NULL;
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
    fepoll_th_data_t *fepoll_ctx = fepoll_th_data_create(fepoll,p);
    *fepoll_ctx->keep_looping = true;
    p->fepoll_ctx = fepoll_ctx;

    frixia_events_queue_t *q = frixia_events_queue_create();
    if ( q == NULL )
    {
        return NULL;
    }
    p->fepoll_events = q;

    p->fdispatcher_ctx = create_frixia_dispatcher_data();
    p->fdispatcher_ctx->dispatcher->tasks = q;


    p->maximum_filedescriptors = maximum_filedescriptors;
    p->filedescriptors = 0;
    return p;
}

int fenv_run_engine(frixia_environment_t *fenv)
{
    fepoll_th_data_t *fepoll_ctx = fenv->fepoll_ctx;
    detached_start_epoll(fepoll_ctx);
    
    frixia_dispatcher_data_t *disp_ctx = fenv->fdispatcher_ctx;
    detached_start_frixia_dispatcher_new(disp_ctx);

    detached_join_epoll(fepoll_ctx);
    detached_join_frixia_dispatcher_new(disp_ctx);

    return 0;
}

void fenv_destroy(frixia_environment_t *fenv)
{
    if ( fenv == NULL )
    {
        printf("Error fenv_destroy pointer is null!\n");
        return;
    }
    fepoll_th_data_t *fepoll_ctx = fenv->fepoll_ctx;
    if ( fepoll_ctx == NULL )
    {
        printf("Error fepoll_ctx pointer is null!\n");
        return;
    }
    destroy_frixia_epoll(fepoll_ctx->fepoll);
}