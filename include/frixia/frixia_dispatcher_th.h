#ifndef DETACHED_FRIXIA_DISPATCHER_NEW_H
#define DETACHED_FRIXIA_DISPATCHER_NEW_H

#include <stdbool.h>
#include <pthread.h>

typedef struct frixia_dispatcher frixia_dispatcher_t;

typedef struct frixia_dispatcher_data_t
{
    pthread_t            thread;
    bool                 started;
    frixia_dispatcher_t *dispatcher;

    void                *ctx;
    
} frixia_dispatcher_data_t;

frixia_dispatcher_data_t *create_frixia_dispatcher_data();
void destroy_frixia_dispatcher_data(frixia_dispatcher_data_t *p);

int detached_start_frixia_dispatcher_new(frixia_dispatcher_data_t *data);
int detached_stop_frixia_dispatcher_new(frixia_dispatcher_data_t *data);
int detached_join_frixia_dispatcher_new(frixia_dispatcher_data_t *data);

#endif