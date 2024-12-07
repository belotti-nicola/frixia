#include <stdlib.h>
#include "../fevent/frixia_events_queue.h"
#include "../fevent/frixia_event.h"
#include "../filedescriptor/reader/filedescriptor_reader.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "errno.h"

#include "frixia_thread_pool.h"

void *thread_main_loop(void *arg)
{
    frixia_thread_pool_data_t *data = (frixia_thread_pool_data_t *)arg;
    frixia_events_queue_t     *q  = data->tasks;
    frixia_epoll_t            *ep = data->fepoll;
    while(true)
    {
        frixia_event_t *e = frixia_events_queue_pop(q);
        int event_fd = e->fd;
        printf("thread main loop!!!%d\n",event_fd);
        char buf[1024+1]; // TODO: DEAL WITH FILE DESCRITOR SIZES
        int bytes_read = read_frixia_filedescriptor(ep,event_fd,buf);
        printf("read: %s(%d bytes read from fd: %d errno %d)\n",buf,bytes_read,event_fd,errno);
        printf("End loop\n");
    }
}

frixia_thread_pool_t* create_frixia_thread_pool(int n,frixia_epoll_t *fepoll, frixia_events_queue_t *events)
{
    frixia_thread_pool_t *ptr = malloc(sizeof(frixia_thread_pool_t));
    if(ptr == NULL)
    {
        printf("ERROR CREATING FRIXIA THREAD POOL POINTER\n");
        return NULL;
    }

    ptr->tasks = NULL;


    frixia_thread_pool_t *t = (frixia_thread_pool_t*)malloc(n*sizeof(frixia_thread_pool_t));
    if (t == NULL)
    {
        printf("ERROR CREATING thread_pool_t");
        return NULL;
    }

    pthread_t *threads = (pthread_t*)malloc(n*sizeof(pthread_t));
    frixia_events_queue_t **queues = (frixia_events_queue_t **)malloc(n*sizeof(frixia_events_queue_t *));
    for(int i=0;i<n;i++)
    {
        pthread_t th;
        frixia_events_queue_t *q = frixia_events_queue_create();
        frixia_thread_pool_data_t *tpdata = create_frixia_thread_pool_data();
        set_frixia_thread_pool_data_events(tpdata,events)
        set_frixia_thread_pool_data_thread_tasks(tpdata,q);
        set_frixia_thread_pool_data_fepoll(tpdata,fepoll);
        int exit_code = pthread_create(&th,NULL,thread_main_loop,tpdata);
        if(exit_code != 0) { exit(-1);}
        threads[i] = th;
        queues[i] = q;
    }
    ptr->th = threads;
    ptr->threads_tasks = queues;
    return ptr;
}

void set_frixia_thread_pool_tasks(frixia_thread_pool_t *ptr, frixia_events_queue_t *tasks)
{
    ptr->tasks = tasks;
}

