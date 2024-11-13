#ifndef FRIXIA_EPOLL_H
#define FRIXIA_EPOLL_H

#include "fepoll_pool.h"

#include "fepoll_codes.h"

typedef struct frixia_epoll 
{
    int                  fd;
    int                  maximum_filedescriptors;
    int                  filedescriptors_number;
    fepoll_pool_t       *fd_pool;

} frixia_epoll_t;

frixia_epoll_t     *create_frixia_epoll();
FRIXIA_EPOLL_CODE_T destroy_frixia_epoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T start_fepoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T stop_fepoll(frixia_epoll_t *fepoll);
FRIXIA_EPOLL_CODE_T insert_into_pool(frixia_epoll_t *fepoll,int fd);


#endif
