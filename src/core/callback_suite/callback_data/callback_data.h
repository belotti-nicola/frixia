#ifndef CALLBACK_DATA_H
#define CALLBACK_DATA_H

#include "../hashmap/hash_map.h"
#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"

typedef struct frixia_callback_data
{
    int        fd;
    HashMap_t *functions;
    HashMap_t *arguments;

} frixia_callback_data_t;

frixia_callback_data_t *create_frixia_callback_data(frixia_event_t *event);
void add_frixia_callback_data(frixia_callback_data_t *cb, void *fun(void *), void *arg);
void destroy_frixia_callback_data_t(frixia_callback_data_t *p);

#endif