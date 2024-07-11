#include <stdlib.h>

#include "frixia_cb_data.h"

frixia_callback_data_t* create_frixia_callback_data(void (*fun)(void *), void *arg)
{
    frixia_callback_data_t* fcb_data = malloc(sizeof(frixia_callback_data_t));
    if(fcb_data == NULL)
    {
        return NULL;
    }
    fcb_data->fun = fun;
    fcb_data->arg = arg;
    return fcb_data;
}

void destroy_frixia_callback_data(frixia_callback_data_t* t)
{
    free(t);
}
