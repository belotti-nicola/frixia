#include <stdbool.h>
#include "frixia_dispatcher.h"

typedef struct frixia_dispatcher_data_t
{
    pthread_t            thread;
    bool                 started;
    frixia_dispatcher_t *dispatcher;

} frixia_dispatcher_data_t;

frixia_dispatcher_data_t *create_frixia_dispatcher_data();
void destroy_frixia_dispatcher_data(frixia_dispatcher_data_t *p);

int detached_start_frixia_dispatcher_new(frixia_dispatcher_data_t *data);
int detached_stop_frixia_dispatcher_new(frixia_dispatcher_data_t *data);
int detached_join_frixia_dispatcher_new(frixia_dispatcher_data_t *data);