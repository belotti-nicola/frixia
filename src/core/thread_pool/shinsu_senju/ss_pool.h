#ifndef SS_POOL_H
#define SS_POOL_H

#include <pthread.h>


typedef struct shinsu_senju_pool 
{
    int size;
    int max_size;

    pthread_t *threads;


} shinsu_senju_pool_t;

shinsu_senju_pool_t *ss_create(int max_threads);
void ss_start_new_thread(shinsu_senju_pool_t *ssp, int key);
void ss_stop_thread(shinsu_senju_pool_t *ssp, int key);
void ss_destroy(shinsu_senju_pool_t *ssp);

#endif