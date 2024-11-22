#ifndef FRIXIA_SUITE_H
#define FRIXIA_SUITE_H

#include "../frixia_common.h"

#include "../../utils/datastructures/simple_list/simple_list.h"
#include "pthread.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"

typedef struct frixia_suite
{
    int max_filedescriptors;
    simple_list_t *fd_pool;
    pthread_t th; 
    frixia_epoll_t *fepoll;

} frixia_suite_t;

frixia_suite_t *create_frixia_suite(int max_dimension);
void            frixia_suite_insert_filedescriptor(frixia_suite_t *s,
                                                   enum FrixiaFDType t,
                                                   int port,
                                                   char *filename,
                                                   int read_dimension);

#endif