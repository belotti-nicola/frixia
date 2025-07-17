#include <stdlib.h>
#include <stdio.h>

#include "simple_valid_callback.h"

sv_callback_t *sv_create_callback(void *(*f)(void *), void *a )
{
    sv_callback_t *ptr = malloc(sizeof(sv_callback_t));
    if ( ptr == NULL )
    {
        printf("Error creating sv_callback_t\n");
    }

    ptr->is_valid = true;
    ptr->function = f;
    ptr->argument = a;

    return ptr;
}

void sv_destroy_callback(sv_callback_t *f)
{
    free(f);
}

void sv_do_callback(sv_callback_t *p)
{
    if( ! p->is_valid )
    {
        printf("sv_do_callback is not valid!\n");
        return;
    }

    if( ! p->function )
    {
        printf("sv_do_callback function is null!\n");
        return;
    }

    void *arg = p->argument;
    callback_func_t *fun = p->function;

    (*fun)(arg);

}

