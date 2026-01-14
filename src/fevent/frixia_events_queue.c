#include "frixia_event.h"
#include <stdlib.h>

#include "frixia_events_queue.h"

frixia_events_queue_t *frixia_events_queue_create()
{
    threadsafe_simple_queue_t *tssq = create_threadsafe_simple_queue();
    if( tssq == NULL)
    {
        printf("ERROR CREATING frixia_events_queue_t(thread safe simple queue)");
        return NULL;
    }

    frixia_events_queue_t *p = malloc(sizeof(frixia_events_queue_t));
    if( p == NULL )
    {
        printf("ERROR CREATING frixia_events_queue_t(thread safe simple queue)");
        return NULL;
    }
    p->queue = tssq;
    return p;
}
void frixia_events_queue_destroy(frixia_events_queue_t *q)
{
    destroy_threadsafe_simple_queue(q->queue);
    free(q);
}
frixia_event_t *frixia_events_queue_pop(frixia_events_queue_t *q)
{
    threadsafe_simple_queue_t *concrete_q = q->queue;
    frixia_event_t *e = (frixia_event_t *)pop_threadsafe_simple_queue(concrete_q);
    return e;
}
void frixia_events_queue_push(frixia_events_queue_t *q,frixia_event_t *event)
{
    threadsafe_simple_queue_t *concrete_q = q->queue;
    push_threadsafe_simple_queue(concrete_q,(void*)event);
}