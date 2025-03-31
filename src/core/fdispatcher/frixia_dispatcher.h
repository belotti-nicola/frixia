#ifndef FRIXIA_DISPATCHER_H
#define FRIXIA_DISPATCHER_H

#include "../fthread_pool/frixia_thread_pool.h"
#include "../fevent/frixia_events_queue.h"
#include "pthread.h"

typedef struct frixia_dispatcher
{
    int                    workers;
    int                    task_index;
    frixia_events_queue_t *tasks;
    frixia_thread_pool_t  *thread_pool;
    int                    stop_fd;

} frixia_dispatcher_t;

frixia_dispatcher_t *create_frixia_dispatcher(int workers,int stop_fd);
void                 destroy_frixia_dispatcher(frixia_dispatcher_t *p);
void                 set_frixia_dispatcher_tasks(frixia_dispatcher_t *disp, frixia_events_queue_t *fqueue);
void                 set_frixia_dispatcher_thread_pool(frixia_dispatcher_t *fd, frixia_thread_pool_t *tp);
void                 dispatch_event_to_workers(frixia_dispatcher_t *dispatcher, frixia_event_t *event);

#endif