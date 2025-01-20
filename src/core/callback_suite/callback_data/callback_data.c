#include "../../../core/frixia_common.h"
#include "../../fevent/frixia_event.h"
#include <stdlib.h>
#include <stdio.h>

#include "callback_data.h"

frixia_callbacks_data_t *create_frixia_callback_data(void (*f)(void *), void *a)
{
    frixia_callbacks_data_t *ptr = malloc(sizeof(frixia_callbacks_data_t));
    if(ptr == NULL)
    {
        printf("frixia_callbacks_data_t creation error!");
        return NULL;
    }
    ptr->function = f;
    ptr->argument = a;
    return ptr;
}
void destroy_frixia_callback_data(frixia_callbacks_data_t *fcbdata)
{
    free(fcbdata);
}
