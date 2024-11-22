#include "pthread.h"
#include "epoll/fepoll.h"
#include <stdio.h>
#include <unistd.h>

#include "detached_epoll_monitor.h"

int frixia_detached_start_monitor(frixia_suite_t *suite)
{   
    pthread_t epoll_thread;
    frixia_epoll_t *fepoll = suite->fepoll;
    int rc = pthread_create( &epoll_thread, NULL, (void *)&start_fepoll, fepoll);
    if(rc != 0) { printf("ERRORCODE::%d\n",rc);}
    suite->th = epoll_thread;
    sleep(5);
    stop_fepoll(fepoll);
    return 0;
}

int frixia_detached_wait_threads(frixia_suite_t *suite)
{
    int rc = pthread_join(suite->th,NULL);
    printf("frixia_detached_wait_threads:: %d\n",rc);
    return 0;
}


