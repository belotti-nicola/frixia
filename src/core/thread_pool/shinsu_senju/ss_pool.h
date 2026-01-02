#ifndef SS_POOL_H
#define SS_POOL_H

#include <pthread.h>
#include "../../fevent/frixia_events_queue.h"


typedef struct shinsu_senju_pool 
{
    int size;
    int max_size;

    pthread_mutex_t        *mutex;
    pthread_cond_t         *no_threads_running;
    frixia_events_queue_t **queues;


} shinsu_senju_pool_t;

shinsu_senju_pool_t *ss_create(int max_threads);
void ss_start_new_thread(shinsu_senju_pool_t *ssp, int key,void *(fun)(void *),void *arg);
void ss_stop_thread(shinsu_senju_pool_t *ssp, int key);
void ss_destroy(shinsu_senju_pool_t *ssp);
void ss_join(shinsu_senju_pool_t *ssp);
void ss_thread_ended(shinsu_senju_pool_t *ssp);
void ss_push_thread(shinsu_senju_pool_t *ssp, int key, void *event);

#endif