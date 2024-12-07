
#ifndef FRIXIA_THREAD_POOL_DATA_H
#define FRIXIA_THREAD_POOL_DATA_H

#include "../fevent/frixia_events_queue.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "pthread.h"

typedef struct frixia_thread_pool_data
{
    frixia_events_queue_t  *events;
    frixia_events_queue_t  *tasks;
    frixia_epoll_t         *fepoll;

} frixia_thread_pool_data_t;

frixia_thread_pool_data_t *create_frixia_thread_pool_data();
void set_frixia_thread_pool_data_events(frixia_thread_pool_data_t *threadpool,
                                       frixia_events_queue_t *events);
void set_frixia_thread_pool_data_thread_tasks(frixia_thread_pool_data_t *threadpool,
                                              frixia_events_queue_t *tasks);
void set_frixia_thread_pool_data_fepoll(frixia_thread_pool_data_t *tp,
                                        frixia_epoll_t *fepoll);

#endif
