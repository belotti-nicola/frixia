#include "ss_wrapper_function.h"
#include "../../fenv/frixia_environment.h"
#include <stdlib.h>

#include "detached_shinsu_senju.h"

void *shinsu_senju_loop_function(void *arg)
{
    shinsu_senju_data_t *ctx = (shinsu_senju_data_t *)arg;
    shinsu_senju_pool_t *ssp = ctx->pool;

    bool *keep_looping = ctx->active;
    while ( *keep_looping )
    {
        ss_join(ssp);
        printf("Empty shinsu_senju");
    }

    return NULL;
}

shinsu_senju_data_t *create_shinsu_senju_data(int workers, void *arg)
{
    shinsu_senju_data_t *retVal = malloc(sizeof(shinsu_senju_data_t));
    if ( retVal == NULL )
    {
        printf("Error creating shinsu senju data!\n");
        return NULL;
    }
    bool *b = malloc(sizeof(bool));
    if ( b == NULL )
    {
        printf("Error creating shinsu senju data boolean!\n");
        return NULL;
    }
    *b = false;
    shinsu_senju_pool_t *ssp = ss_create(workers);

    retVal->maximum_workers = workers;
    retVal->pool = ssp;
    retVal->active = b;
    retVal->ctx = arg;
    return retVal;
}

int detached_start_shinsu_senju(shinsu_senju_data_t *shinsu_senju)
{
    pthread_t ss_thread;
    int rc = pthread_create( &ss_thread,
                             NULL,
                             shinsu_senju_loop_function,
                             shinsu_senju);
    if(rc != 0) 
    { 
        printf("ERRORCODE1::%d\n",rc);
        return 0;
    }

    shinsu_senju->th = ss_thread;
    *(shinsu_senju->active) = true;
    return 0;
}
int detached_stop_shinsu_senju(shinsu_senju_data_t *shinsu_senju)
{
    bool *b = shinsu_senju->active;
    *b = false;

    int dim = shinsu_senju->maximum_workers;
    for (int i=0;i<dim;i++)
    {
        frixia_events_queue_t *q = *(shinsu_senju->pool->queues + i);
        frixia_events_queue_push(q,NULL);
    }
}
int detached_join_shinsu_senju(shinsu_senju_data_t *shinsu_senju)
{
    pthread_t th = shinsu_senju->th;
    pthread_join(th,NULL);
}

void detached_shinsu_senju_load(shinsu_senju_data_t *ssd, int key,void *(fun)(void *), void *arg)
{
    shinsu_senju_pool_t *ssp = ssd->pool;
    frixia_events_queue_t *q = *(ssp->queues + key);
    ss_worker_ctx_t ctx = 
    {
        .id = key,
        .keep_looping = ssd->active,
        .events = q,
        .shinsu_senju = ssp
    };

    ss_start_new_thread(ssp,key,fun,(void *)&ctx);
}

void detached_shinsu_senju_push(shinsu_senju_data_t *ssd, int key, void *event)
{
    frixia_events_queue_t *q = *(ssd->pool->queues + key);
    if (!q )
    {
        return;
    }
    frixia_events_queue_push(q,NULL);
}
