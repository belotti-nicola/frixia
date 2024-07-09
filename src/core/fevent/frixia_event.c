#include "frixia_event.h"
#include "../frixia_common.h"
#include <stdlib.h>

frixia_event_t* create_event(enum FrixiaFDType type,
                             char* data,
                             int reply_fd)
{
    frixia_event_t* fe = malloc(sizeof(frixia_event_t));
    if(fe == NULL) 
    {
        return NULL;
    }
    fe->data     = data;
    fe->reply_fd = reply_fd;
    fe->type     = type;
    return fe;
}

void destroy_event(frixia_event_t *e)
{
    free(e);
}
