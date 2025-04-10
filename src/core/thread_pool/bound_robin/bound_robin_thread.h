#ifndef BOUND_ROBIN_THREAD
#define BOUND_ROBIN_THREAD


#include "../../../utils/datastructures/threadsafe_simple_queue/threadsafe_simple_queue.h"
#include <stdatomic.h>

#include "bound_robin_thread.h"


typedef struct thread_context
{
    threadsafe_simple_queue_t *thread_events;
    atomic_bool               *keep_looping;

    void                      *(*cb_main)(void *);
    void                      *cb_arg;

} thread_context_t;

typedef struct bound_robin_thread
{
    thread_context_t *ctx;


} bound_robin_thread_t;

void bound_robin_thread_main_loop(void *ctx);
void bound_robin_thread_stop(thread_context_t *ctx);
thread_context_t *bound_robin_create_thread_context();
#endif