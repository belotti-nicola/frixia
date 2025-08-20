#ifndef FCTX_H
#define FCTX_H

#include <stdint.h>
#include "fepoll_ctx.h"
#include "fdispatcher_ctx.h"
#include "fevent_ctx.h"

typedef struct fctx
{
    fevent_ctx_t *event_ctx;
    fepoll_ctx_t *fepoll_ctx;
    fdispatcher_ctx_t *fdispatcher_ctx;
    //fthread_pool_ctx_t *fthread_pool_ctx;

} fctx_t;

#endif