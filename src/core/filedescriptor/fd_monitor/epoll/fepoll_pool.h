#ifndef FEPOLL_POOL_H
#define FEPOLL_POOL_H

#include "../../../../utils/datastructures/simple_list/simple_list.h"

typedef struct fepoll_fd 
{
    int fd;
} fepoll_fd_t;

typedef struct fepoll_pool
{
    simple_list_t *l;
} fepoll_pool_t;


#endif