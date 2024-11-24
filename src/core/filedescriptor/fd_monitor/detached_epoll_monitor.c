#include "pthread.h"
#include "epoll/fepoll_loop_function.h"
#include <stdio.h>
#include <unistd.h>

#include "detached_epoll_monitor.h"

int frixia_detached_start_monitor(frixia_suite_t *suite)
{   
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread,
                             NULL,
                             (void *)&fepoll_loop_function,
                             suite);
    if(rc != 0) { printf("ERRORCODE::%d\n",rc);}
    return 0;
}

int frixia_detached_wait_threads(frixia_suite_t *suite)
{
    int rc = pthread_join(suite->th,NULL);
    printf("frixia_detached_wait_threads:: %d\n",rc);
    return 0;
}


