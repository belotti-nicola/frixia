#include "../../protocols/frixia_supported_protocols.h"
#include <stdlib.h>
#include <stdio.h>

#include "frixia_callback_entry.h"

frixia_callback_entry_t *create_frixia_callback_entry(int fd, enum FrixiaFDType type, FRIXIA_SUPPORTED_PROTOCOL_T protocol,void *data)
{
    frixia_callback_entry_t *ptr = malloc(sizeof(frixia_callback_entry_t));
    if(ptr == NULL)
    {
        printf("ERROR CREATING FRIXIA_CALLBACK_ENTRY_T\n");
        return NULL;
    }

    ptr->fd       = fd;
    ptr->type     = type;
    ptr->protocol = protocol;
    ptr->data     = data;

    return ptr;
}
void destroy_frixia_callback_entry(frixia_callback_entry_t *p)
{
    free(p);
}
