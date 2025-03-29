#include "pthread.h"
#include "epoll/fepoll_loop_function.h"
#include <stdio.h>
#include <unistd.h>
#include "epoll/fepoll.h"

#include "detached_epoll_monitor.h"

int frixia_detached_start_monitor(fepoll_th_data_t *th_data)
{   
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread,
                             NULL,
                             (void *)&fepoll_loop_function,
                             th_data);
    th_data->th = epoll_thread;
    if(rc != 0) 
    { 
        printf("ERRORCODE1::%d\n",rc);
    }
    return 0;
}

int frixia_detached_wait_monitor(fepoll_th_data_t *th_data)
{
    int rc = pthread_join(th_data->th,NULL);
    if(rc != 0) 
    {
        printf("ERRORCODE2::%d\n",rc);
    }
    return 0;
}


