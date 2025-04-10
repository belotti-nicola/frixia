#include "bound_robin_event.h"

bound_robin_event_t bound_robin_create_event(void *(*fun)(void *),void *arg)
{
    bound_robin_event_t ev;
    ev.fun = fun;
    ev.arg = arg;

    return ev;
}
