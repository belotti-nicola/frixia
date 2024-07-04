#include "frixia_event.h"
#include "../frixia_common.h"
#include <stdlib.h>

frixia_event_t* create_event(enum FrixiaFDType t, int reply)
{
    frixia_event_t* fe = malloc(sizeof(frixia_event_t));
    if(fe == NULL) 
    {
        return NULL;
    }
    fe->reply_fd = reply;
    fe->type = t;
    return fe;
}

void destroy_event(frixia_event_t *e)
{
    free(e);
}
