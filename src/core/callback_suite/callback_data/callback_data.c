#include "callback_data.h"
#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"
#include "../hashmap/hash_map.h"

frixia_callback_data_t *create_frixia_callback_data(frixia_event_t *event)
{
    frixia_callback_data_t *p = malloc(sizeof(frixia_callback_data_t));
    if( p == NULL )
    {
        return NULL;
    }
    
    HashMap_t *hm_1 = create_hash_map(64);
    if( hm_1 == NULL )
    {
        free(p);
        return NULL;
    }
    HashMap_t *hm_2 = create_hash_map(64);
    if( hm_2 == NULL )
    {
        free(p);
        free(hm_1);
        return NULL;
    }
    p->fd = event->fd;
    return p;
}

void destroy_frixia_callback_data(frixia_callback_data_t *p)
{
    destroy_hash_map(p->arguments);
    destroy_hash_map(p->arguments);
    free(p);
}

void add_frixia_callback_data(frixia_callback_data_t *cb, void *fun(void *), void *arg)
{
    HashMap_t *cb_functions = cb->functions;
    if(cb_functions == NULL)
    {
        cb_functions = create_hash_map(64);
    }
    HashEntry_t *he_fun = create_hash_entry("aaaa",he_fun);
    add_entry(cb_functions,he_fun);

    HashMap_t *cb_arguments = cb->arguments;
    if(cb_arguments == NULL)
    {
        cb_arguments = create_hash_map(64);
    }
    HashEntry_t *he_arg = create_hash_entry("bbbb",he_arg);
    add_entry(cb_arguments,he_arg);
    
}
