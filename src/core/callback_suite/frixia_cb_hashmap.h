#ifndef FRIXIA_CB_HASHMAP_H
#define FRIXIA_CB_HASHMAP_H

#include "callback_data/callback_data_structure.h"
#include "../../setup/proto_callbacks/proto_cb.h"
typedef struct frixia_callbacks_suite
{
    int                     size;
    int                     max_size;
    frixia_callback_data_t *cb_data;

} frixia_callbacks_suite_t;

frixia_callbacks_suite_t *create_frixia_callbacks_suite(int size);
void                      frixia_callbacks_suite_add(frixia_callbacks_suite_t *t,
                                                     proto_frixia_callback_t *p);
void                      destroy_frixia_callbacks_suite(frixia_callbacks_suite_t *t);

#endif