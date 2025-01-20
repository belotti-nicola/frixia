#ifndef CALLBACK_DATA_H
#define CALLBACK_DATA_H

#include "../../../core/frixia_common.h"

typedef struct frixia_callback_data
{
    void                            (*function)(void *);
    void                             *argument;

} frixia_callbacks_data_t;

frixia_callbacks_data_t *create_frixia_callback_data(void (*callback)(void *), void *argument);
void                     destroy_frixia_callback_data(frixia_callbacks_data_t *fcbdata);

#endif