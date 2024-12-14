#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"
#include "../hashmap/hash_map.h"

#include "callback_data.h"

frixia_callback_data_t *create_frixia_callback_data(frixia_event_t *event,int callbacks_size)
{
    frixia_callback_data_t *p = malloc(sizeof(frixia_callback_data_t));
    if( p == NULL )
    {
        return NULL;
    }
    
    HashMap_t *hm_1 = create_hash_map(callbacks_size);
    if( hm_1 == NULL )
    {
        free(p);
        return NULL;
    }
    HashMap_t *hm_2 = create_hash_map(callbacks_size);
    if( hm_2 == NULL )
    {
        free(p);
        free(hm_1);
        return NULL;
    }
    p->fd = event->fd;
    p->arguments = hm_1;
    p->functions = hm_2;
    return p;
}

void destroy_frixia_callback_data(frixia_callback_data_t *p)
{
    destroy_hash_map(p->arguments);
    destroy_hash_map(p->arguments);
    free(p);
}

void add_frixia_callback_data(frixia_callback_data_t *cb, char *key, void *fun(void *), void *arg)
{
    HashEntry_t *hashed_fun = create_hash_entry(key,fun);
    HashMap_t *cb_functions = cb->functions;
    add_entry(cb_functions,hashed_fun);

    HashEntry_t *hashed_arg = create_hash_entry(key,arg);
    HashMap_t *cb_arguments = cb->arguments;
    add_entry(cb_arguments,hashed_arg);
}
