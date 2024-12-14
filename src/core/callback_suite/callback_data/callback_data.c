#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"
#include "../hashmap/hash_map.h"
#include "hashed_callback.h"

#include "callback_data.h"

frixia_callbacks_data_t *create_frixia_callback_data(frixia_event_t *event,int callbacks_size)
{
    frixia_callbacks_data_t *p = malloc(sizeof(frixia_callbacks_data_t));
    if( p == NULL )
    {
        return NULL;
    }
    
    HashMap_t *hm= create_hash_map(callbacks_size);
    if( hm == NULL )
    {
        free(p);
        return NULL;
    }

    p->fd        = event->fd;
    p->callbacks = hm;
    return p;
}

void destroy_frixia_callback_data(frixia_callbacks_data_t *p)
{
    destroy_hash_map(p->callbacks);
    free(p);
}

void add_frixia_callback_data(frixia_callbacks_data_t *cb, char *key, void (*fun)(void *), void *arg)
{
    hashed_callback_t *hcb = create_hashed_callback(fun,arg);
    HashEntry_t *hashed_cb = create_hash_entry(key,hcb);
    HashMap_t *cb_functions = cb->callbacks;
    add_entry(cb_functions,hashed_cb);
}
