#include "fepoll_context.h"

void *fepoll_context_push_event(void *arg)
{
    fepoll_ctx_t *ctx = (fepoll_ctx_t *)arg;
    if( ctx == NULL )
    {
        printf("Error! CTX IS NULL");
        return NULL;
    }
    
    frixia_events_queue_t *q = ctx->events;
    frixia_event_t *ev = create_event(ctx->fd);
    frixia_events_queue_push(q,ev);

    return NULL;
}
void *fepoll_context_stop(void *arg)
{
    fepoll_ctx_t *ctx = (fepoll_ctx_t *)arg;
    if( ctx == NULL )
    {
        printf("Error! CTX IS NULL");
        return NULL;
    }
    
    bool *keep_looping = ctx->keep_looping;
    *keep_looping = false;
    
    return NULL;
}

void *fepoll_context_counter(void *arg)
{
    fepoll_ctx_t *ctx = (fepoll_ctx_t *)arg;
    if( ctx == NULL )
    {
        printf("Error! CTX IS NULL");
        return NULL;
    }

    int *a = ctx->counter;

    *a = *a + 1;
    printf("counter: %d\n",*a);
    return NULL;
}