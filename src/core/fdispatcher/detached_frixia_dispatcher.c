#include "frixia_dispatcher.h"
#include <stdlib.h>
#include "frixia_dispatcher_loop_function.h"

#include "detached_frixia_dispatcher.h"

waitable_frixia_dispatcher_t *create_waitable_frixia_dispatcher(int workers)
{
    waitable_frixia_dispatcher_t *wfd = malloc(sizeof(waitable_frixia_dispatcher_t));
    if(wfd == NULL)
    {
        printf("ERROR CREATING WAITABLE FRIXIA DISPATCHER\n");
        return NULL;
    }

    frixia_dispatcher_t *disp = create_frixia_distpatcher(workers);
    wfd->dispatcher = disp;

    return wfd;
}

int detached_start_frixia_dispatcher(waitable_frixia_dispatcher_t *dispatcher)
{
    pthread_t th;
    int rc = pthread_create(&th,
                            NULL,
                            (void *)&frixia_dispatcher_loop_function,
                            dispatcher->dispatcher);
    dispatcher->thread = &th;
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