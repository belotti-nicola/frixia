#include <stdlib.h>
#include <stdio.h>
#include "dispatcher/frixia_dispatcher.h"
#include "dispatcher/frixia_dispatcher_loop_function.h"

#include <frixia/frixia_dispatcher_th.h>

frixia_dispatcher_data_t *create_frixia_dispatcher_data()
{
    frixia_dispatcher_data_t *p = malloc(sizeof(frixia_dispatcher_data_t));
    if ( p == NULL ) 
    {
        printf("Error frixia_dispatcher_data_t NULL\n");
        return NULL;
    }

    p->dispatcher = create_frixia_dispatcher(1);
    p->started = false;
    
    return p;
}

void destroy_frixia_dispatcher_data(frixia_dispatcher_data_t *p)
{
    frixia_dispatcher_t *disp = p->dispatcher;
    destroy_frixia_dispatcher(disp);
    free(p);
}

int detached_start_frixia_dispatcher_new(frixia_dispatcher_data_t *data)
{
    pthread_t th;
    void *(*casted_fun)(void *) = (void *(*)(void *))frixia_dispatcher_loop_function;
    int rc = pthread_create(&th,
                            NULL,
                            casted_fun,
                            (void *)data);
    if (rc != 0)
    {
        printf("ERROR::detached_start_frixia_dispatcher::RC %d\n",rc);
        return -1;
    }

    data->thread  = th;
    data->started = true;
    return 0;
}
int detached_stop_frixia_dispatcher_new(frixia_dispatcher_data_t *data)
{
    if ( data == NULL )
    {
        printf("WRN::detached_stop_frixia_dispatcher::pointer is null.\n");
        return -1;
    }
    
    if ( ! data->started)
    {
        printf("WRN::detached_stop_frixia_dispatcher::thread wasn't started.\n");
        return -1;
    }

    int  thread_return = 0;
    int *ptr = &thread_return;

    pthread_t th = data->thread;
    int rc = pthread_join(th,(void **)&ptr);
    if (rc != 0)
    {
        printf("ERROR::detached_stop_frixia_dispatcher::rc %d\n",rc);
        return -1;
    }
    return thread_return;
}
int detached_join_frixia_dispatcher_new(frixia_dispatcher_data_t *data)
{
        if ( data == NULL )
    {
        printf("WRN::detached_stop_frixia_dispatcher::pointer is null.\n");
        return -1;
    }
    
    if ( ! data->started)
    {
        printf("WRN::detached_stop_frixia_dispatcher::thread wasn't started.\n");
        return -1;
    }

    int  thread_return = 0;
    int *ptr = &thread_return;

    pthread_t th = data->thread;
    int rc = pthread_join(th,(void **)&ptr);
    if (rc != 0)
    {
        printf("ERROR::detached_stop_frixia_dispatcher::rc %d\n",rc);
        return -1;
    }
    return thread_return;
}