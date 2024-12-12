#ifndef CALLBACK_DATA_H
#define CALLBACK_DATA_H

#include "../hashmap/hash_map.h"
#include "../../../core/frixia_common.h"

typedef struct frixia_callback_data
{
    int        fd;
    HashMap_t *functions;
    HashMap_t *arguments;

} frixia_callback_data_t;

frixia_callback_data_t *create_frixia_callback_data(enum FrixiaFDType fdtype,
                                                    int port,
                                                    char *filname,
                                                    HashMap_t *hm);
void destroy_frixia_callback_data_t(frixia_callback_data_t *p);

#endif