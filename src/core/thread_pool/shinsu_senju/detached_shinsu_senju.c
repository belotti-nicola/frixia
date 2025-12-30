#include "detached_shinsu_senju.h"

void *shinsu_senju_loop_function(void *arg)
{
    shinsu_senju_data_t *ctx = (shinsu_senju_data_t *)arg;
    shinsu_senju_pool_t *ssp = ctx->pool;

    bool *keep_looping = ctx->active;
    while ( * keep_looping )
    {
        ss_join(ssp);
        printf("Empty shinsu_senju");
    }

    return NULL;
}

shinsu_senju_data_t *create_shinsu_senju_data(int workers)
{
    shinsu_senju_data_t *retVal = malloc(sizeof(shinsu_senju_pool_t));
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
    return retVal;
}

int detached_start_shinsu_senju(shinsu_senju_data_t *shinsu_senju)
{
    pthread_t ss_thread;
    int rc = pthread_create( &ss_thread,
                             NULL,
                             (void *)shinsu_senju_loop_function,
                             shinsu_senju->active);
    if(rc != 0) 
    { 
        printf("ERRORCODE1::%d\n",rc);
        return 0;
    }

    shinsu_senju->th = ss_thread;
    shinsu_senju->active = true;
    return 0;
}
int detached_stop_shinsu_senju(shinsu_senju_data_t *shinsu_senju)
{
    return 0;
}
int detached_join_shinsu_senju(shinsu_senju_data_t *shinsu_senju)
{
    return 0;
}

void detached_shinsu_senju_load(shinsu_senju_data_t *ssd, int key,void *(fun)(void *), void *arg)
{
    shinsu_senju_pool_t *ssp = ssd->pool;
    ss_start_new_thread(ssp,key,fun,arg);
}
