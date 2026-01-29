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
    bool *keep_looping;

    void *context;

    sv_callback_t *callbacks;

} fepoll_th_data_t;



fepoll_th_data_t *fepoll_th_data_create(void *ctx);
void *fepoll_th_data_destroy(fepoll_th_data_t *p);

int detached_start_epoll(fepoll_th_data_t *fepoll);
int detached_stop_epoll(fepoll_th_data_t *fepoll);
int detached_join_epoll(fepoll_th_data_t *fepoll);

void register_callback_by_fd(fepoll_th_data_t *th_data, int fd, sv_callback_t *sv );
void fepoll_register_push_callback(fepoll_th_data_t *fepoll,int fd);


#endif