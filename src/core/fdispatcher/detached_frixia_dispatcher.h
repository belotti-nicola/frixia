#include "frixia_dispatcher.h"

typedef struct waitable_frixia_dispatcher
{
    pthread_t           *thread;
    frixia_dispatcher_t *dispatcher;

} waitable_frixia_dispatcher_t;

void create_waitable_frixia_dispatcher(waitable_frixia_dispatcher_t *d,int workers);
int detached_start_frixia_dispatcher(waitable_frixia_dispatcher_t *dispatcher);
int detached_stop_frixia_dispatcher(waitable_frixia_dispatcher_t *dispatcher);