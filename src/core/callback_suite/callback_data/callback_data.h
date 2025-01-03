#ifndef CALLBACK_DATA_H
#define CALLBACK_DATA_H

#include "../hashmap/hash_map.h"
#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"

typedef struct frixia_callback_data
{
    int                               fd;
    enum FRIXIA_SUPPORTED_PROTOCOL    protocol;
    void                            (*callback)(void *);
    void                             *argument;

} frixia_callbacks_data_t;

frixia_callbacks_data_t *create_frixia_callback_data(int fd, enum FRIXIA_SUPPORTED_PROTOCOL  protocol, void (*callback)(void *), void *argument);
void                     destroy_frixia_callback_data(frixia_callbacks_data_t *fcbdata);

#endif