#include <stdlib.h>
#include <internal/ss_pool.h>
#include <frixia/frixia_environment.h>

//TODO CLEAN
#include <signal.h>

#include <frixia/ss_wrapper_function.h>

void *ss_worker_function(void *arg)
{
    ss_worker_ctx_t *ctx = (ss_worker_ctx_t *)arg;
    
    printf("Worker %d started...\n",ctx->fd);
    bool *keep_looping = ctx->keep_looping;
    frixia_events_queue_t *q = ctx->events;
    shinsu_senju_data_t *ssd = ctx->shinsu_senju_ctx;
    sv_callback_t sv = ctx->callback;

    frixia_event_t *e;
    void *(*fun)(void *) = sv.function;
    while( *keep_looping )
    {
        e = frixia_events_queue_pop(q);
        if ( e == NULL )
        {
            printf("Empty ss_worker_function\n");
            continue;
        }
        fun(ctx);
        printf("Callback done\n");
    }

    shinsu_senju_pool_t *ssp = ssd->pool;
    ss_thread_ended(ssp);
    printf("Worker %d ended...\n",ctx->fd);
    destroy_ss_worker_ctx(ctx);
    return NULL;
}

ss_worker_ctx_t *create_ss_worker_ctx(int fd, bool *kl, frixia_events_queue_t *e, shinsu_senju_data_t *ssd,sv_callback_t sv, frixia_environment_t *fenv)
{
    ss_worker_ctx_t *ctx = malloc(sizeof(ss_worker_ctx_t));
    if ( ctx == NULL )
    {
        printf("Error! ctx worker is null\n");
        return NULL;
    }

    ctx->fd = fd;
    ctx->events = e;
    ctx->keep_looping = kl;
    ctx->shinsu_senju_ctx = ssd;
    ctx->callback = sv;
    ctx->fenv = fenv;
    return ctx;
}

void destroy_ss_worker_ctx(ss_worker_ctx_t *ctx)
{
    free(ctx);
}
