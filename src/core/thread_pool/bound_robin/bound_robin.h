#ifndef BOUND_ROBIN_H
#define BOUND_ROBIN_H

#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include "../../../core/frixia_common.h"
#include <pthread.h>

#include "bound_robin.h"


typedef struct bound_robin
{
    int                        current_index;
    threadsafe_simple_queue_t *event_in[FRIXIA_WORKERS];
    pthread_t                 *th[FRIXIA_WORKERS];

} bound_robin_t;

int bound_robin_add_task(bound_robin_t *br, void *task);
int bound_robin_broadcast_task(bound_robin_t *br, void *task);

#endif