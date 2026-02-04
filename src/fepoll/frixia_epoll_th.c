#include <internal/convoy.h>
#include <stdlib.h>
#include <stdio.h>
#include "epoll/fepoll.h"
#include "epoll/fepoll_loop_function.h"
#include <frixia/frixia_environment.h>

#include <internal/frixia_epoll_th.h>

fepoll_th_data_t *fepoll_th_data_create(frixia_environment_t *fenv)
{
    fepoll_th_data_t *p = malloc(sizeof(fepoll_th_data_t));
    if ( p == NULL )
    {
        printf("Error creating  pointer\n");
        return NULL;
    }

    frixia_epoll_t *fepoll = create_frixia_epoll();
    if ( p == NULL )
    {
        printf("Error creating fepoll instance\n");
        return NULL;
    }
    p->fepoll = fepoll;

    bool *b = malloc(sizeof(bool));
    if ( b == NULL )
    {
        printf("Error creating fepoll b\n");
        return NULL;
    }
    *b = true;
    p->keep_looping = b;

    sv_callback_t *sv = calloc(25,sizeof(sv_callback_t));
    if ( sv == NULL )
    {
        return NULL;
    }
    p->fenv = fenv;
    p->callbacks = sv;
    return p;
}
void *fepoll_th_data_destroy(fepoll_th_data_t *p)
{
    destroy_frixia_epoll(p->fenv->fepoll_ctx->fepoll);
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
        printf("detached_start_epoll ERRORCODE1::%d\n",rc);
        return 0;
    }
    fepoll_obj->th = epoll_thread;

    return 0;
}

int detached_stop_epoll(fepoll_th_data_t *fepoll_obj)
{   
    bool *b = fepoll_obj->keep_looping;
    *b = false;
    //frixia_epoll_t *fepoll = fepoll_obj->fepoll;
    //frixia_wake(fepoll);
    return 0;
}

int detached_join_epoll(fepoll_th_data_t *fepoll_obj)
{
    pthread_t th = fepoll_obj->th;
    int rc = pthread_join(th, NULL);
    if(rc != 0) 
    { 
        printf("detached_start_epoll ERRORCODE2::%d\n",rc);
    }
    return rc;
}



