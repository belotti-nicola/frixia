#include <stdlib.h>
#include <stdio.h>

#include "frixia_callback.h"

frixia_callback_t *create_frixia_callback(void *(*function)(void *),void *argument)
{
    frixia_callback_t *ret = malloc(sizeof(frixia_callback_t));
    if ( ret == NULL )
    {
        printf("Error frixia_callback!!!\n");
        return NULL;
    }
    ret->argument = argument;
    ret->function = function;

    return ret;
}