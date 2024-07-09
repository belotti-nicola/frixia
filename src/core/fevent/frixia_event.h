#ifndef FRIXIA_FEVENT_H
#define FRIXIA_FEVENT_H

#include "../frixia_common.h"

typedef struct frixia_event
{
    char *data;
    enum FrixiaFDType type;
    int reply_fd;

} frixia_event_t;

frixia_event_t *create_event(enum FrixiaFDType t,
                             char *data,
                             int reply);
void destroy_event(frixia_event_t *e);

#endif