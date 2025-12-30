#include <stdlib.h>
#include <stdio.h>

#include "ss_pool.h"

shinsu_senju_pool_t *ss_create(int max_threads)
{
    shinsu_senju_pool_t *ssp = malloc(sizeof(shinsu_senju_pool_t));
    if ( ssp == NULL )
    {
        printf("Error SSP!\n");
        return NULL;
    }

    pthread_t *threads = malloc( sizeof( pthread_t ) * max_threads );
    if ( threads == NULL )
    {
        printf("Error SSP thread allocation!\n");
        return NULL;
    }
    ssp->max_size = max_threads;
    ssp->size     = 0;
    ssp->threads  = threads;
    return ssp;
}
void ss_start_new_thread(shinsu_senju_pool_t *ssp, int key,void *(fun)(void *),void *arg)
{
    pthread_t th;
    int rc = pthread_create(&th,fun,arg,NULL);
    if ( rc != 0 )
    {
        printf("Error ss_start_new_thread!!!\n");
        return;
    }
    
    ssp->size+=1;
}
void ss_stop_thread(shinsu_senju_pool_t *ssp, int key)
{
    return;
}

void ss_join(shinsu_senju_pool_t *ssp)
{
    //pthread_cond_wait()
}

void ss_destroy(shinsu_senju_pool_t *ssp)
{
    if ( ssp == NULL )
    {
        return;
    }
    free(ssp);
}