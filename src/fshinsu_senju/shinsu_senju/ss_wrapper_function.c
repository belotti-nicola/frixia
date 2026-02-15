#include <stdlib.h>
#include <internal/ss_pool.h>
#include <frixia/frixia_environment.h>
#include <frixia/frixia_callbacks.h>

//TODO CLEAN
#include <signal.h>

#include <frixia/ss_wrapper_function.h>

void *ss_worker_function(void *arg)
{
    FRIXIA_CALLBACK_CTX *ctx = (FRIXIA_CALLBACK_CTX *)arg;
    
    int fd = ctx->fd;
    printf("Worker %d started...\n",fd);
    bool *keep_looping = ctx->keep_looping;
    frixia_events_queue_t **queues = ctx->fenv->shinsu_senju_ctx->pool->queues;
    frixia_events_queue_t  *q      = *(queues + fd); 
    shinsu_senju_data_t *ssd = ctx->fenv->shinsu_senju_ctx;
    sv_callback_t sv = ctx->sv;

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

FRIXIA_CALLBACK_CTX *create_ss_worker_ctx(int fd, bool *kl, frixia_events_queue_t *e, shinsu_senju_data_t *ssd,sv_callback_t sv, frixia_environment_t *fenv)
{
    FRIXIA_CALLBACK_CTX *ctx = malloc(sizeof(FRIXIA_CALLBACK_CTX));
    if ( ctx == NULL )
    {
        printf("Error! ctx worker is null\n");
        return NULL;
    }

    ctx->fd = fd;
    ctx->keep_looping = kl;//todo: inner keep looping masking
    ctx->sv = sv;
    ctx->fenv = fenv;
    return ctx;
}

void destroy_ss_worker_ctx(FRIXIA_CALLBACK_CTX *ctx)
{
    free(ctx);
}
