#include <stdlib.h>

#include "frixia_cb_hashmap.h"

frixia_callbacks_suite_t *create_frixia_callbacks_suite(int size)
{
    frixia_callbacks_suite_t *fsuite = malloc(sizeof(frixia_callbacks_suite_t));
    if (fsuite == NULL)
    {
        return NULL;
    }
    fsuite->size = size;
    return fsuite;
}

void frixia_callbacks_suite_add(frixia_callbacks_suite_t *suite,
                                void (*function)(void *),
                                void *argument)
{
    frixia_callback_data_t *cb_data = create_frixia_callback_data(function, argument);
    //hash_map_add(suite,cb_data);
    
}

void destroy_frixia_callbacks_suite(frixia_callbacks_suite_t *t)
{   
    int dim = t->size;
    free(t);
}
