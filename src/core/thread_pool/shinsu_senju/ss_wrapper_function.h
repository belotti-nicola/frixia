#ifndef SS_WRAPPER_FUNCTION_H
#define SS_WRAPPER_FUNCTION_H

#include "../../../utils/valid_callback/simple_valid_callback.h"
#include "../../../core/fevent/frixia_events_queue.h"
#include "detached_shinsu_senju.h"

typedef struct ss_worker_ctx_t
{
    int id;
    bool *keep_looping;
    frixia_events_queue_t *events;
    shinsu_senju_data_t *shinsu_senju_ctx;
    sv_callback_t callback;

    void *arg;
   
} ss_worker_ctx_t;

ss_worker_ctx_t *create_ss_worker_ctx(int id, bool *kl, frixia_events_queue_t *e, shinsu_senju_data_t *ssp,sv_callback_t sv,void *arg);
void destroy_ss_worker_ctx(ss_worker_ctx_t *ctx);

void *ss_worker_function(void *arg);

#endif