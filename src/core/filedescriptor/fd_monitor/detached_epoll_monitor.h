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



fepoll_th_data_t *fepoll_th_data_create(frixia_epoll_t *fepoll);
void *fepoll_th_data_destroy(fepoll_th_data_t *p);

int detached_start_epoll(fepoll_th_data_t *fepoll);
int detached_join_epoll(fepoll_th_data_t *fepoll);

#endif