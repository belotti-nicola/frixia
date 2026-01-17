#ifndef FCTX_H
#define FCTX_H

#include <stdint.h>
#include "event_context.h"
#include <frixia/frixia_environment.h>

typedef struct fctx
{
    event_ctx_t *ev_ctx;
    frixia_environment_t *env;
    void *arg;

} fctx_t;

#endif