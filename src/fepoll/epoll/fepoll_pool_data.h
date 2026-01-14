#ifndef FEPOLL_POOL_DATA_H
#define FEPOLL_POOL_DATA_H

#include "../../../frixia_common.h"
typedef struct fepoll_data
{
    int               fd;
    enum FrixiaFDType info;

} fepoll_data_t;


fepoll_data_t *create_fepoll_data(int fd);
void           destroy_fepoll_data(fepoll_data_t *p);

#endif