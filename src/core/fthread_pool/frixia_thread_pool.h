#ifndef FRIXIA_THREAD_POOL_H
#define FRIXIA_THREAD_POOL_H

#include "../fevent/frixia_events_queue.h"
#include "frixia_thread_pool_data.h"
#include "pthread.h"

typedef struct frixia_thread_pool
{
    int                         workers;
    pthread_t                  *th;
    frixia_events_queue_t      *tasks;
    frixia_events_queue_t     **threads_tasks;
    frixia_thread_pool_data_t  *thpool_data;

} frixia_thread_pool_t;

frixia_thread_pool_t *create_frixia_thread_pool(int n,frixia_epoll_t *fepoll,frixia_events_queue_t *events);
void                  set_frixia_thread_pool_tasks(frixia_thread_pool_t *ptr,
                                                   frixia_events_queue_t *tasks);


#endif
