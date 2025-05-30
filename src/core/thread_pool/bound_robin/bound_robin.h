#ifndef BOUND_ROBIN_H
#define BOUND_ROBIN_H

#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include "../../../core/frixia_common.h"
#include "bound_robin_thread.h"
#include "bound_robin_event.h"
#include <pthread.h>
#include <stdbool.h>

#include "bound_robin.h"


typedef struct bound_robin
{
    int                        current_index;
    pthread_t                  th[FRIXIA_WORKERS];
    pthread_barrier_t          *create_barrier;

    thread_context_t          *th_contex[FRIXIA_WORKERS];

} bound_robin_t;

void bound_robin_create(bound_robin_t *br);

void bound_robin_add_task_to_one_worker(bound_robin_t *br, void *(*function)(void *), void *argument);
void bound_robin_add_task_to_all_workers(bound_robin_t *br, void *(*function)(void *), void *argument);
void bound_robin_stop(bound_robin_t *br);
void bound_robin_wait(bound_robin_t *br);

#endif