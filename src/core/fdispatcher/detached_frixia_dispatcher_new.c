#include "frixia_dispatcher_loop_function.h"
#include <stdio.h>
#include "frixia_dispatcher.h"

#include "detached_frixia_dispatcher_new.h"

int detached_start_frixia_dispatcher_new(frixia_dispatcher_data_t *data)
{
    pthread_t th;
    frixia_dispatcher_t *d = data->dispatcher;

    void (*casted_fun)(void *) = (void (*)(void *))frixia_dispatcher_loop_function;
    int rc = pthread_create(&th,
                            NULL,
                            casted_fun,
                            (void *)d);
    if (rc != 0)
    {
        printf("ERROR::detached_start_frixia_dispatcher::RC %d\n",rc);
        return -1;
    }

    data->thread = th;
    data->started = true;
    return 0;
}
int detached_join_frixia_dispatcher_new(frixia_dispatcher_data_t *data)
{
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