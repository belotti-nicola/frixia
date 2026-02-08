#include <stdlib.h>
#include <stdio.h>

#include <frixia/ss_wrapper_function.h>
#include <internal/ss_pool.h>

shinsu_senju_pool_t *ss_create(int max_threads)
{
    shinsu_senju_pool_t *ssp = malloc(sizeof(shinsu_senju_pool_t));
    if ( ssp == NULL )
    {
        printf("Error SSP!\n");
        return NULL;
    }

    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    if ( mutex == NULL )
    {
        printf("Error pthread_mutex_t mutex!\n");
        return NULL;
    }

    pthread_cond_t *no_threads_running = malloc(sizeof(pthread_cond_t));;
    if ( no_threads_running == NULL )
    {
        printf("Error pthread_cond_t empty!\n");
        return NULL;
    }

    frixia_events_queue_t **queues = malloc(max_threads * sizeof(frixia_events_queue_t *) );
    if ( queues == NULL )
    {
        printf("Error SSP QUEUES!\n");
        return NULL;
    }
    for( int i=0; i<max_threads;i++)
    {
        
    }

    ssp->max_size = max_threads;
    ssp->size     = 0;
    ssp->mutex    = mutex;
    ssp->queues   = queues;

    ssp->no_threads_running = no_threads_running;
    return ssp;
}
void ss_start_new_thread(shinsu_senju_pool_t *ssp, int key,void *(fun)(void *),void *arg)
{
    pthread_t th;
    int rc = pthread_create(&th,NULL,ss_worker_function,arg);
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

void ss_destroy(shinsu_senju_pool_t *ssp)
{
    if ( ssp == NULL )
    {
        return;
    }
    free(ssp);
}

void ss_thread_ended(shinsu_senju_pool_t *ssp)
{
    pthread_mutex_t *mutex = ssp->mutex;
    pthread_mutex_lock(mutex);
    
    ssp->size-=1;
    if ( ssp->size == 0 )
    {
        pthread_cond_t *cond = ssp->no_threads_running;
        pthread_cond_signal(cond);
    }

    pthread_mutex_unlock(mutex);
}

void ss_thread_started(shinsu_senju_pool_t *ssp)
{
    pthread_mutex_t *mutex = ssp->mutex;
    pthread_mutex_lock(mutex);

    ssp->size+=1;
    
    pthread_mutex_unlock(mutex);
}

void ss_join(shinsu_senju_pool_t *ssp)
{
    pthread_mutex_t *mutex = ssp->mutex;
    pthread_mutex_lock(mutex);

    pthread_cond_t *no_threads_running = ssp->no_threads_running;
    pthread_cond_wait(no_threads_running,mutex);

    pthread_mutex_unlock(mutex);
}


void ss_push_thread(shinsu_senju_pool_t *ssp, int key, void *event)
{
    frixia_events_queue_t **queues = ssp->queues;
    frixia_events_queue_t  *queue  = *(queues + key);

    frixia_events_queue_push(queue,event);
}
