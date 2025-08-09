#include "fepoll_context.h"

void fepoll_context_push_event(fepoll_ctx_t *ctx)
{
    if( ctx == NULL )
    {
        printf("Error! CTX IS NULL");
        return;
    }
    
    frixia_events_queue_t *q = ctx->events;
    frixia_event_t *ev = create_event(ctx->fd);
    frixia_events_queue_push(q,ev);

    return;
}
void fepoll_context_stop(fepoll_ctx_t *ctx)
{
    if( ctx == NULL )
    {
        printf("Error! CTX IS NULL");
        return;
    }
    
    bool *keep_looping = ctx->keep_looping;
    *keep_looping = false;
    
    return;
}