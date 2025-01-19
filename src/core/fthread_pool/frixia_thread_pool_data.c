#include "frixia_thread_pool.h"
#include "../callback_suite/callback_data/callback_data.h"
#include "../fevent/frixia_events_queue.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../callback_suite/callback_data/frixia_callbacks.h"
#include <stdio.h>
#include <stdlib.h>

#include "frixia_thread_pool_data.h"

frixia_thread_pool_data_t *create_frixia_thread_pool_data()
{
    frixia_thread_pool_data_t *ptr = malloc(sizeof(frixia_thread_pool_data_t));
    if(ptr == NULL)
    {
        printf("ERROR creating thread pool data\n");
        return NULL;
    }
    return ptr;
}

void destroy_frixia_thread_pool_data(frixia_thread_pool_data_t *t)
{
    free(t);
}

void set_frixia_thread_pool_data_events(frixia_thread_pool_data_t *tp,
                                       frixia_events_queue_t *q)
{
    tp->events = q;
}

void set_frixia_thread_pool_data_thread_tasks(frixia_thread_pool_data_t *tp,
                                              frixia_events_queue_t *tt)
{
    tp->tasks = tt;
}

void set_frixia_thread_pool_data_fepoll(frixia_thread_pool_data_t *tp,
                                        frixia_epoll_t *fe)
{
    tp->fepoll = fe;
}

void set_frixia_thread_pool_data_callbacks(frixia_thread_pool_data_t         *tp,
                                           frixia_callbacks_data_structure_t *cbs)
{
    tp->frixia_callbacks = cbs;
}
