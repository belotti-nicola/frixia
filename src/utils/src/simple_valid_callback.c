#include <stdlib.h>
#include <stdio.h>

#include <simple_valid_callback.h>

sv_callback_t *sv_create_callback(void *(*f)(void *), void *c )
{
    sv_callback_t *ptr = malloc(sizeof(sv_callback_t));
    if ( ptr == NULL )
    {
        printf("Error creating sv_callback_t\n");
    }

    ptr->is_valid = true;
    ptr->function = f;
    ptr->auxiliary = c;

    return ptr;
}

void sv_destroy_callback(sv_callback_t *f)
{
    free(f);
}

bool sv_is_valid(sv_callback_t *cb)
{
    if ( cb == NULL )
    {
        return false;
    }
    return cb->is_valid;
}