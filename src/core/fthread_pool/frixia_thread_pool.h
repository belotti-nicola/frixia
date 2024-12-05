#ifndef FRIXIA_THREAD_POOL_H
#define FRIXIA_THREAD_POOL_H

#include "../fevent/frixia_events_queue.h"
#include "pthread.h"

typedef struct frixia_thread_pool
{
    int                      workers;
    frixia_events_queue_t   *tasks;
    pthread_t               *th;
    frixia_events_queue_t  **threads_tasks;


} frixia_thread_pool_t;

frixia_thread_pool_t *create_frixia_thread_pool(int n);
void                  set_frixia_thread_pool_tasks(frixia_thread_pool_t *ptr,
                                                   frixia_events_queue_t *tasks);


#endif
