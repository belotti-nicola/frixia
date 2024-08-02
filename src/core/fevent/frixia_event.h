#ifndef FRIXIA_FEVENT_H
#define FRIXIA_FEVENT_H

#include "../frixia_common.h"
#include "../protocols/frixia_supported_protocols.h"

typedef struct frixia_event
{
    int                         fd;
    enum FrixiaFDType           fd_type;
    FRIXIA_SUPPORTED_PROTOCOL_T protocol;
} frixia_event_t;

frixia_event_t *create_event(int                         fd,
                             enum FrixiaFDType           fd_type,
                             FRIXIA_SUPPORTED_PROTOCOL_T protocol);
void destroy_event(frixia_event_t *e);

#endif