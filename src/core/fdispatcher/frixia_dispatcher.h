#ifndef FRIXIA_DISPATCHER_H
#define FRIXIA_DISPATCHER_H

#include "../fthread_pool/frixia_thread_pool.h"
#include "../fevent/frixia_events_queue.h"
#include "../thread_pool/bound_robin/bound_robin.h"
#include "../convoy/convoy.h"
#include "pthread.h"
#include <stdbool.h>

typedef struct frixia_dispatcher
{
    int                    workers;
    int                    task_index;
    frixia_events_queue_t *tasks;
    frixia_thread_pool_t  *thread_pool;
    bound_robin_t         *bound_robin;
    int                    stop_fd;
    bool                  *keep_looping;
    convoy_t              *convoy;

} frixia_dispatcher_t;

frixia_dispatcher_t *create_frixia_dispatcher(int workers,int stop_fd);
void                 destroy_frixia_dispatcher(frixia_dispatcher_t *p);
void                 set_frixia_dispatcher_tasks(frixia_dispatcher_t *disp, frixia_events_queue_t *fqueue);
void                 set_frixia_dispatcher_thread_pool(frixia_dispatcher_t *fd, frixia_thread_pool_t *tp);
void                 set_frixia_dispatcher_bound_robin(frixia_dispatcher_t *fd, bound_robin_t *br);
void                 dispatch_event_to_workers(frixia_dispatcher_t *dispatcher, frixia_event_t *event);
void                 dispatch_event_to_all_workers(frixia_dispatcher_t *dispatcher,frixia_event_t *event);
void                 dispatcher_stop(frixia_dispatcher_t *dispatcher);

#endif