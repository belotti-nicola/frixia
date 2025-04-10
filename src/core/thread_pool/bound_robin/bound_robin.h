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
    void                      *(*delegate_function)(void *);
    void                      *delegate_argument;
    pthread_barrier_t          *create_barrier;

    thread_context_t          *th_contex[FRIXIA_WORKERS];

} bound_robin_t;

void bound_robin_create(bound_robin_t *br,
                        void *th_main_fun(void *),void *th_main_arg,
                        void *th_delegate_fun(void *),void *th_delegate_arg);

void bound_robin_add_task(bound_robin_t *br, bound_robin_event_t *e);
void bound_robin_broadcast_task(bound_robin_t *br, bound_robin_event_t *e);
void bound_robin_wait(bound_robin_t *br);

#endif