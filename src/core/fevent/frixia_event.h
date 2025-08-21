#ifndef FRIXIA_FEVENT_H
#define FRIXIA_FEVENT_H

#include "../frixia_common.h"
#include <stdint.h>
#include "../protocols/frixia_supported_protocols.h"
#include <stdint.h>

typedef struct frixia_event
{
    int fd;
    uint32_t events_maks;

} frixia_event_t;

frixia_event_t *create_event(int fd, uint32_t mask);
void destroy_event(frixia_event_t *e);

#endif