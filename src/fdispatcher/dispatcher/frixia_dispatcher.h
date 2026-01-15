#ifndef FRIXIA_DISPATCHER_H
#define FRIXIA_DISPATCHER_H

#include <internal/frixia_epoll_th.h>
#include <internal/frixia_events_queue.h>
#include <internal/convoy.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct frixia_dispatcher
{
    int                    workers;
    int                    task_index;
    frixia_events_queue_t *tasks;
    int                    stop_fd;
    bool                  *keep_looping;
    convoy_t              *convoy;
    frixia_epoll_t        *fepoll;

} frixia_dispatcher_t;

frixia_dispatcher_t *create_frixia_dispatcher(int workers);
void                 destroy_frixia_dispatcher(frixia_dispatcher_t *p);

#endif