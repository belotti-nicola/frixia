#ifndef FRIXIA_EVENTS_QUEUE_H
#define FRIXIA_EVENTS_QUEUE_H

#include <threadsafe_simple_queue.h>

#include "frixia_event.h"

typedef struct frixia_events_queue 
{
    threadsafe_simple_queue_t *queue;

} frixia_events_queue_t;

frixia_events_queue_t *frixia_events_queue_create();
void                   frixia_events_queue_destroy(frixia_events_queue_t *q);
frixia_event_t        *frixia_events_queue_pop(frixia_events_queue_t *q);
void                   frixia_events_queue_push(frixia_events_queue_t *q,frixia_event_t *event);

#endif