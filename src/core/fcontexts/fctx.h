#ifndef FCTX_H
#define FCTX_H

#include <stdint.h>
#include "event_context.h"

typedef struct fctx
{
    event_ctx_t *ev_ctx;
    //fepoll_ctx_t *fepoll_ctx;
    //fdispatcher_ctx_t *fdispatcher_ctx;
    //fthread_pool_ctx_t *fthread_pool_ctx;

} fctx_t;

#endif