#ifndef FEPOLL_POOL_H
#define FEPOLL_POOL_H

#include "../../../../utils/datastructures/simple_list/simple_list.h"
#include "fepoll_pool_data.h"

typedef struct fepoll_pool
{
    simple_list_t *l;

} fepoll_pool_t;

fepoll_pool_t *create_fepoll_pool();
void           destroy_fepoll_pool(fepoll_pool_t *p);
void           fepoll_pool_add_fd(fepoll_pool_t *p, int fd);
void           fepoll_pool_remove_fd(fepoll_pool_t *p, int index);


#endif