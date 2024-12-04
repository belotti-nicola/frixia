#include <pthread.h>
#include "detached_frixia_dispatcher.h"
#include "frixia_dispatcher.h"

#include "frixia_dispatcher_loop_function.h"

void create_waitable_frixia_dispatcher(waitable_frixia_dispatcher_t *d,int workers)
{
    d->dispatcher->workers = workers;
}

int detached_start_frixia_dispatcher(waitable_frixia_dispatcher_t *dispatcher)
{
    int rc = pthread_create( dispatcher->thread,
                             NULL,
                             (void *)&frixia_dispatcher_loop_function,
                             dispatcher
                            );
    if(rc != 0) { printf("ERRORCODE1::%d\n",rc);}
    return 0;
}

int detached_stop_frixia_dispatcher(waitable_frixia_dispatcher_t *wait_disp)
{
    pthread_t *t = wait_disp->thread;
    int rc = pthread_join(*t,NULL);
    if(rc != 0) { printf("ERRORCODE2::%d\n",rc);}
    return 0;
}