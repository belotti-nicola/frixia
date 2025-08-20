#ifndef FDISPATCHER_CTX_H
#define FDISPATCHER_CTX_H

#include <stdint.h>
#include <stdbool.h>
#include "../fevent/frixia_events_queue.h"

typedef struct fdispatcher_ctx
{
    frixia_events_queue_t     *in;
    bool                      *keep_looping;

} fdispatcher_ctx_t;

void *fdispatcher_context_push_event(void *arg);
void *fdispatcher_context_stop(void *arg);
void *fdispatcher_context_counter(void *arg);

#endif