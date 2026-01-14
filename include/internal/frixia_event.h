#ifndef FRIXIA_EVENT_H
#define FRIXIA_EVENT_H

#include <stdint.h>

typedef struct frixia_event
{
    int fd;
    uint32_t events_maks;

} frixia_event_t;

frixia_event_t *create_event(int fd, uint32_t mask);
void destroy_event(frixia_event_t *e);

#endif