
#include "../../../utils/datastructures/simple_list/simple_list.h"
#include "../../fevent/frixia_event.h"
#include "callback_data.h"
#include <stdlib.h>
#include <stdio.h>

#include "frixia_callbacks.h"

frixia_callbacks_t *create_frixia_callbacks_data_structure()
{
    frixia_callbacks_t *p = malloc(sizeof(frixia_callbacks_data_t));
    if(p == NULL)
    {
        printf("ERROR malloc frixia_callbacks_t");
        return NULL;
    }
    simple_list_t *l = create_simple_list();
    if(l == NULL)
    {
        return NULL;
    }

    p->events_callbacks = l;
    return p;
}
void destroy_frixia_callbacks_data_structure(frixia_callbacks_t *d)
{
    destroy_simple_list(d->events_callbacks);
    free(d);
}
void add_entry_frixia_callbacks_data_structure(
    frixia_callbacks_t   *datastructure,
    frixia_event_t       *event,
    char                 *key,
    void                (*fun)(void *),
    void                 *arg
    )
{
    simple_list_t          *l    = datastructure->events_callbacks;
    simple_list_elem_t     *curr = l->first;
    frixia_callbacks_data_t *cb;

    while( curr != NULL )
    {
        cb = curr->val;
        if( cb->fd == event->fd)
        {
            add_frixia_callback_data(cb,key,fun,arg);
            return;           
        }
        curr = curr->next;
    }

    cb = create_frixia_callback_data(event,64);
    add_item(l,cb);
    //i could relaunch the function but
    //i fear endless loop
    frixia_callbacks_data_t *just_added = (frixia_callbacks_data_t *)l->last->val;
    add_frixia_callback_data(cb,key,fun,arg);
}