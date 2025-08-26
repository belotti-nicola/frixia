#ifndef FCTX_H
#define FCTX_H

#include <stdint.h>
#include "event_context.h"
#include "../fenv/frixia_environment.h"

typedef struct fctx
{
    event_ctx_t *ev_ctx;
    frixia_environment_t *env;
    void *arg;
    //fdispatcher_ctx_t *fdispatcher_ctx;
    //fthread_pool_ctx_t *fthread_pool_ctx;

} fctx_t;

#endif