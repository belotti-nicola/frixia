#include "pthread.h"
#include "epoll/fepoll_loop_function.h"
#include <stdio.h>
#include <unistd.h>
#include "epoll/fepoll.h"
#include <string.h>
#include <stdlib.h>

#include "detached_epoll_monitor.h"

fepoll_th_data_t *fepoll_th_data_create(frixia_epoll_t *fepoll, frixia_environment_t *fenv)
{
    fepoll_th_data_t *p = malloc(sizeof(fepoll_th_data_t));
    if ( p == NULL )
    {
        printf("Error creating fepoll\n");
        return NULL;
    }

    p->fepoll = fepoll;
    p->keep_looping = true;
    p->fenv = fenv;

    return p;
}
void *fepoll_th_data_destroy(fepoll_th_data_t *p)
{
    free(p);
}

int detached_start_epoll(fepoll_th_data_t *fepoll_obj)
{   
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread,
                             NULL,
                             (void *)&fepoll_loop_function,
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
    fepoll_obj->keep_looping = false;
    return 0;
}

int detached_join_epoll(fepoll_th_data_t *fepoll_obj)
{
    pthread_join(fepoll_obj->th, NULL);
}


