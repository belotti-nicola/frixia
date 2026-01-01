#ifndef SS_WRAPPER_FUNCTION_H
#define SS_WRAPPER_FUNCTION_H

#include "../../../utils/valid_callback/simple_valid_callback.h"
#include "../../../core/fevent/frixia_events_queue.h"
#include "../../../core/fenv/frixia_environment.h"
#include "ss_pool.h"

typedef struct ss_worker_ctx_t
{
    int id;
    bool *keep_looping;
    frixia_events_queue_t *events;
    frixia_environment_t *fenv;
    shinsu_senju_pool_t *shinsu_senju;

    void *arg;
   
} ss_worker_ctx_t;

void *ss_worker_function(void *arg);


#endif