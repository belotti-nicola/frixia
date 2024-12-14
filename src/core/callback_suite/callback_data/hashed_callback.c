#include <stdio.h>
#include <stdlib.h>

#include "hashed_callback.h"

hashed_callback_t *create_hashed_callback(void *a, void (*f)(void *))
{
    hashed_callback_t *p = malloc(sizeof(hashed_callback_t));
    if(p == NULL)
    {
        printf("ERROR CREATING HASHED CALLBACK\n");
        return NULL;
    }
    p->argument = a;
    p->function = f;
    return p;
}
void destroy_hashed_callback(hashed_callback_t *p)
{
    free(p);
}