#ifndef FRIXIA_DISPATCHER_H
#define FRIXIA_DISPATCHER_H

#include "../fevent/frixia_events_queue.h"
#include "pthread.h"

typedef struct
{
    int                    workers;
    frixia_events_queue_t *tasks;

} frixia_dispatcher_t;

frixia_dispatcher_t *create_frixia_distpatcher(int workers);
void                 destroy_frixia_dispatcher(frixia_dispatcher_t *p);
void                 set_frixia_dispatcher_tasks(frixia_dispatcher_t *q, frixia_events_queue_t *fq);

#endif