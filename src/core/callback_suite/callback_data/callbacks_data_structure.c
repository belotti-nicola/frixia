
#include "../../../utils/datastructures/simple_list/simple_list.h"
#include "callback_data.h"
#include <stdlib.h>
#include <stdio.h>

#include "callbacks_data_structure.h"

frixia_callbacks_data_structure_t *create_frixia_callbacks_data_structure()
{
    frixia_callbacks_data_structure_t *p = malloc(sizeof(frixia_callback_data_t));
    if(p == NULL)
    {
        printf("ERROR malloc frixia_callbacks_data_structure_t");
        return NULL;
    }
    simple_list_t *l = create_simple_list();
    if(l == NULL)
    {
        return NULL;
    }

    p->callbacks = l;
    return p;
}
void destroy_frixia_callbacks_data_structure(frixia_callbacks_data_structure_t *d)
{
    destroy_simple_list(d->callbacks);
    free(d);
}
void add_entry_frixia_callbacks_data_structure(
    frixia_callbacks_data_structure_t *datastructure,
    frixia_event_t                    *event,
    void                              *fun(void *),
    void                              *arg
    )
{
}