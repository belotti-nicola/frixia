#ifndef FRIXIA_FEVENT_H
#define FRIXIA_FEVENT_H

#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"

typedef struct frixia_event
{
    int fd;

} frixia_event_t;

frixia_event_t *create_event(int fd);
void destroy_event(frixia_event_t *e);

#endif