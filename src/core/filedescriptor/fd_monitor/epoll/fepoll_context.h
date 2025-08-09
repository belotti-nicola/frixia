#ifndef FEPOLL_CONTEXT_H
#define FEPOLL_CONTEXT_H

#include <stdint.h>
#include "../../../fevent/frixia_events_queue.h"

typedef struct fepoll_ctx
{
    int fd;
    uint32_t mask;

    frixia_events_queue_t     *events;
    bool                      *keep_looping;

} fepoll_ctx_t;


void fepoll_context_push_event(fepoll_ctx_t *ctx);
void fepoll_context_stop(fepoll_ctx_t *ctx);

#endif