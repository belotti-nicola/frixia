#ifndef FEPOLL_H
#define FEPOLL_H

#include "../../../../core/fsuite/frixia_fd.h"
#include "../../../fevent/frixia_events_queue.h"
#include "fepoll_codes.h"
#include "fepoll_pool.h"


typedef struct frixia_epoll 
{
    int                      fd;
    int                      maximum_filedescriptors;
    int                      filedescriptors_number;
    fepoll_pool_t           *fd_pool;
    int                      stop_fd;
    frixia_events_queue_t   *events_queue;

} frixia_epoll_t;

frixia_epoll_t     *create_frixia_epoll();
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T fadd_stop_filedescriptor(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T stop_fepoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fepoll,int fd);
FRIXIA_EPOLL_CODE_T insert_event(int epoll, frixia_fd_t *f);
int                 frixia_epoll_wait(frixia_epoll_t *fepoll, frixia_event_t *fevents);

#endif

