#ifndef BOUND_ROBIN_EVENT_H
#define BOUND_ROBIN_EVENT_H

typedef struct bound_robin_event
{
    void *(*fun)(void *);
    void *arg;

} bound_robin_event_t;


bound_robin_event_t *bound_robin_create_event(void *(*fun)(void *),void *arg);

#endif