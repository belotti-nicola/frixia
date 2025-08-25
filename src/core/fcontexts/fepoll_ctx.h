#ifndef FEPOLL_CTX_H
#define FEPOLL_CTX_H

#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include <stdbool.h>

typedef struct fepoll_ctx_t
{
    frixia_epoll_t *fepoll;
    bool           *keep_looping;

} fepoll_ctx_t;

#endif