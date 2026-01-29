#include "pthread.h"
#include "epoll/fepoll_loop_function.h"
#include <stdio.h>
#include <unistd.h>
#include "epoll/fepoll.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../fenv/frixia_environment.h"
#include "../../../core/frixia_h.h"

#include "detached_epoll_monitor.h"

fepoll_th_data_t *fepoll_th_data_create(void *ctx)
{
    fepoll_th_data_t *p = malloc(sizeof(fepoll_th_data_t));
    if ( p == NULL )
    {
        printf("Error creating fepoll\n");
        return NULL;
    }

    frixia_epoll_t *fepoll = create_frixia_epoll();
    if ( fepoll == NULL )
    { 
        printf("Error creating fepoll instance\n");
        return NULL;
    }


    bool *b = malloc(sizeof(bool));
    if ( b == NULL )
    {
        printf("Error creating fepoll b\n");
        return NULL;
    }
    *b = true;

    
    sv_callback_t *sv = calloc(MAXIMUM_FD_NUMBER,sizeof(sv_callback_t));
    if ( sv == NULL )
    {
        return NULL;
    }

    p->fepoll = fepoll;
    p->context = (void *)ctx;
    p->keep_looping = b;
    p->started = false;
    p->callbacks = sv;
    p->fepoll = fepoll;
    return p;
}
void *fepoll_th_data_destroy(fepoll_th_data_t *p)
{
    destroy_frixia_epoll(p->fepoll);
    free(p->keep_looping);
    free(p);
}

int detached_start_epoll(fepoll_th_data_t *fepoll_obj)
{   
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread,
                             NULL,
                             (void *)fepoll_loop_function,
                             fepoll_obj);
    if(rc != 0) 
    { 
        printf("ERRORCODE1::%d\n",rc);
        fepoll_obj->started = false;
        return 0;
    }

    fepoll_obj->th      = epoll_thread;
    fepoll_obj->started = true;

    return 0;
}

int detached_stop_epoll(fepoll_th_data_t *fepoll_obj)
{   
    bool *b = fepoll_obj->keep_looping;
    *b = false;
    frixia_epoll_t *fepoll = fepoll_obj->fepoll;
    frixia_wake(fepoll);
    return 0;
}

int detached_join_epoll(fepoll_th_data_t *fepoll_obj)
{
    pthread_t th = fepoll_obj->th;
    int rc = pthread_join(th, NULL);
    if(rc != 0) 
    { 
        printf("ERRORCODE2 for epoll::%d\n",rc);
    }
    return rc;
}

void register_callback_by_fd(fepoll_th_data_t *th_data, int fd, sv_callback_t *sv )
{
    sv_callback_t *callbacks = th_data->callbacks;
    callbacks[fd] = *sv;
}

void fepoll_register_push_callback(fepoll_th_data_t *fepoll,int fd)
{
    frixia_environment_t *fenv = (frixia_environment_t *)fepoll->context;
    
    frixia_events_queue_t *q = fenv->fepoll_events;
    fepoll_th_data_t *fep_data = fenv->fepoll_ctx;
    sv_callback_t *sv = sv_create_callback(handle_fepoll_push,q);
    register_callback_by_fd(fep_data,fd,sv);


    int epoll_fd = fenv->fepoll_ctx->fepoll->fd;
    insert_event(epoll_fd,fd);
}

