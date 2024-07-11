#ifndef FRIXIA_CB_HASHMAP_H
#define FRIXIA_CB_HASHMAP_H

#include "frixia_cb_data.h"

typedef struct frixia_callbacks_suite
{
    int                     size;
    frixia_callback_data_t *callback_data;

} frixia_callbacks_suite_t;

frixia_callbacks_suite_t *create_frixia_callbacks_suite(int size);
void                      frixia_callbacks_suite_add(frixia_callbacks_suite_t *t,
                                                     void (*function)(void *),
                                                     void *argument);
void                      destroy_frixia_callbacks_suite(frixia_callbacks_suite_t *t);

#endif