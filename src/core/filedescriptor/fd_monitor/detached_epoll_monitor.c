#include "pthread.h"
#include "epoll/fepoll.h"
#include <stdio.h>

#include "detached_epoll_monitor.h"

int frixia_detached_start_monitor(frixia_suite_t *suite)
{   
    
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread, NULL, (void *)&start_fepoll, suite);
    if(rc != 0) { printf("ERRORCODE::%d\n",rc);}
    suite->th = epoll_thread;
    return 0;
}

int frixia_detached_wait_threads(frixia_suite_t *suite)
{
    return pthread_join(suite->th,NULL);
}


