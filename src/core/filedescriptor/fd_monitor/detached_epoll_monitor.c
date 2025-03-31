#include "pthread.h"
#include "epoll/fepoll_loop_function.h"
#include <stdio.h>
#include <unistd.h>
#include "epoll/fepoll.h"
#include <string.h>

#include "detached_epoll_monitor.h"

int frixia_detached_start_monitor(fepoll_th_data_t *th_data)
{   
    pthread_t epoll_thread;
    int rc = pthread_create( &epoll_thread,
                             NULL,
                             (void *)&fepoll_loop_function,
                             th_data);
    if(rc != 0) 
    { 
        printf("ERRORCODE1::%d\n",rc);
        th_data->started = false;
        return 0;
    }

    th_data->th      = epoll_thread;
    th_data->started = true;

    return 0;
}

int frixia_detached_wait_monitor(fepoll_th_data_t *th_data)
{
    if( ! th_data->started )
    {
        return -1;
    }

    int thread_return = 0;
    int *ptr = &thread_return;
    
    pthread_t thread = th_data->th;  
    int rc = pthread_join(thread,(void **)&ptr);
    if(rc != 0) 
    {
        printf("ERRORCODE2::%d\n",rc);
        return -1;
    }

    return thread_return;
}


