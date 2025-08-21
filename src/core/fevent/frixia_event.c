#include "frixia_event.h"
#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"
#include <stdlib.h>

frixia_event_t* create_event(int fd, uint32_t mask)
{
    if(fd < 0)
    {
        return NULL;
    }
    frixia_event_t* fe = malloc(sizeof(frixia_event_t));
    if(fe == NULL) 
    {
        return NULL;
    }
    fe->fd = fd;
    fe->events_maks = mask;
    
    return fe;
}

void destroy_event(frixia_event_t *e)
{
    free(e);
}
