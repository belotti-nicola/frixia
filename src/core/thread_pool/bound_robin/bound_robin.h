#ifndef BOUND_ROBIN_H
#define BOUND_ROBIN_H

#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include "../../../core/frixia_common.h"
#include <pthread.h>

#include "bound_robin.h"

typedef struct bound_robin
{
    int                        current_index;
    pthread_t                  th[FRIXIA_WORKERS];
    void                      *(*delegate_function)(void *);
    void                      *delegate_argument;

} bound_robin_t;

void bound_robin_create(bound_robin_t *br,
                                 void *th_fun(void *),
                                 void *th_arg,
                                 void *th_delegate_fun(void *),
                                 void *th_delegate_arg);

void bound_robin_add_task(bound_robin_t *br, void *task);
void bound_robin_broadcast_task(bound_robin_t *br, void *task);
void delegate_target_thread_with_policy(bound_robin_t *br,int index,void *task);
void bound_robin_wait(bound_robin_t *br);

#endif