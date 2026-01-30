#include <internal/ss_pool.h>
#include <frixia/ss_wrapper_function.h>
#include "shinsu_senju/shinsu_senju_loop_function.h"
#include <stdlib.h>

#include <frixia/frixia_shinsu_senju_th.h>
#include <frixia/frixia_environment.h>

shinsu_senju_data_t *create_shinsu_senju_data(int workers, frixia_environment_t *fenv)
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
    *b = true;
    shinsu_senju_pool_t *ssp = ss_create(workers);

    retVal->maximum_workers = workers;
    retVal->pool = ssp;
    retVal->active = b;
    retVal->fenv = fenv;
    return retVal;
}
void destroy_shinsu_senju_data(shinsu_senju_data_t *ssd)
{
    bool *b = ssd->active;
    free(b);
    ss_destroy(ssd->pool);
    free(ssd);
}

int  detached_shinsu_senju_start(shinsu_senju_data_t *fshinsu_senju)
{
    pthread_t ss_thread;
    int rc = pthread_create( &ss_thread,
                             NULL,
                             shinsu_senju_loop_function,
                             fshinsu_senju);
    if(rc != 0) 
    { 
        printf("ERRORCODE1::%d\n",rc);
        return 0;
    }

    fshinsu_senju->th = ss_thread;
    return 0;
}
int  detached_shinsu_senju_stop(shinsu_senju_data_t *fshinsu_senju)
{
    bool *b = fshinsu_senju->active;
    *b = false;

    int dim = fshinsu_senju->maximum_workers;
    for (int i=0;i<dim;i++)
    {
        frixia_events_queue_t *q = *(fshinsu_senju->pool->queues + i);
        frixia_events_queue_push(q,NULL);
    }
}
int  detached_shinsu_senju_join(shinsu_senju_data_t *fshinsu_senju)
{
    pthread_t th = fshinsu_senju->th;
    pthread_join(th,NULL);
}
void detached_shinsu_senju_load(shinsu_senju_data_t *ssd,int key,void *(fun)(void *), void *arg)
{
    shinsu_senju_pool_t *ssp = ssd->pool;
    frixia_events_queue_t **q = ssp->queues + key;
    bool *keep_looping = ssd->active;
    void *fenv = ssd->fenv;
    sv_callback_t sv = 
    {
        .is_valid = true,
        .function = fun,
        .auxiliary = arg
    };
    
    ss_worker_ctx_t *ctx = create_ss_worker_ctx(key,keep_looping,*q, ssd,sv,fenv);
    ss_start_new_thread(ssp,key,fun,ctx);
}
void detached_shinsu_senju_push(shinsu_senju_data_t *ssd, int key, void *event)
{
    shinsu_senju_pool_t *ssp = ssd->pool;
    ss_push_thread(ssp,key,event);
}