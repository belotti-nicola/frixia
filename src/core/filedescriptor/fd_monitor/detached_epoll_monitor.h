#ifndef DETACHED_EPOLL_MONITOR
#define DETACHED_EPOLL_MONITOR

#include "../../convoy/convoy.h"
#include "../../filedescriptor/fd_monitor/epoll/fepoll.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct fepoll_th_data
{
    convoy_t *convoy;
    frixia_epoll_t *fepoll;
    frixia_events_queue_t *events;
    pthread_t th;
    bool started;

} fepoll_th_data_t;

int frixia_detached_start_monitor(fepoll_th_data_t *th_data);
int frixia_detached_wait_monitor(fepoll_th_data_t *th_data);

#endif