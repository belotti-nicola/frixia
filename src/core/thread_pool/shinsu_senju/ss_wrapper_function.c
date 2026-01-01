#include "ss_wrapper_function.h"


void *ss_worker_function(void *arg)
{
    ss_worker_ctx_t *ctx = (ss_worker_ctx_t *)arg;
    
    bool *keep_looping = ctx->keep_looping;
    frixia_events_queue_t *q = ctx->events;
    frixia_event_t *e;
    while( *keep_looping )
    {
        e = frixia_events_queue_pop(q);
        printf("Popped\n");
        //TODO CALLBACK
    }

    shinsu_senju_pool_t *ssp = ctx->shinsu_senju;
    ss_thread_ended(ssp);
    return NULL;
}