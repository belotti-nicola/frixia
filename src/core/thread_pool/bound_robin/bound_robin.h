#ifndef BOUND_ROBIN_H
#define BOUND_ROBIN_H

#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include "../../../core/frixia_common.h"
#include <pthread.h>
#include <stdbool.h>

#include "bound_robin.h"

typedef struct thread_context
{
    threadsafe_simple_queue_t *thread_events;
    bool                      *keep_looping;
    void                      *thread_data;

    void                      *(*cb_main)(void *);
    void                      *cb_arg;

} thread_context_t;

typedef struct callback_arg
{
    void *event;
    void *client_code;
    
} callback_arg_t;

typedef struct bound_robin
{
    int                        current_index;
    pthread_t                  th[FRIXIA_WORKERS];
    void                      *(*delegate_function)(void *);
    void                      *delegate_argument;

    thread_context_t          *th_contex[FRIXIA_WORKERS];

} bound_robin_t;

void bound_robin_create(bound_robin_t *br,
                        void *th_main_fun(void *),void *th_main_arg,
                        void *th_delegate_fun(void *),void *th_delegate_arg);

void bound_robin_add_task(bound_robin_t *br, void *task);
void bound_robin_broadcast_task(bound_robin_t *br, void *task);
void bound_robin_wait(bound_robin_t *br);

#endif