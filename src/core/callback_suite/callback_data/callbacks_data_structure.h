#ifndef CALLBACKS_DATA_STRUCTURE_H
#define CALLBACKS_DATA_STRUCTURE_H

#include "../../../utils/datastructures/simple_list/simple_list.h"
#include "../../fevent/frixia_event.h"
#include "callback_data.h"

typedef struct frixia_callbacks_data_structure
{
    simple_list_t *callbacks;

} frixia_callbacks_data_structure_t;

frixia_callbacks_data_structure_t *create_frixia_callbacks_data_structure();
void                               destroy_frixia_callbacks_data_structure(frixia_callbacks_data_structure_t *d);
void                               add_entry_frixia_callbacks_data_structure(
    frixia_callbacks_data_structure_t *datastructure,
    frixia_event_t                    *event,
    void                              *fun(void *),
    void                              *arg
    );

#endif
