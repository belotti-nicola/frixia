#ifndef CALLBACK_DATA_H
#define CALLBACK_DATA_H

#include "../hashmap/hash_map.h"
#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"

typedef struct frixia_callback_data
{
    int        fd;
    HashMap_t *callbacks;

} frixia_callbacks_data_t;

frixia_callbacks_data_t *create_frixia_callback_data(frixia_event_t *event,int callbacks_size);
void add_frixia_callback_data(frixia_callbacks_data_t *cb, char *key, void (*fun)(void *), void *arg);
void destroy_frixia_callbacks_data_t(frixia_callbacks_data_t *p);

#endif