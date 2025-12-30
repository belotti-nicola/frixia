
#ifndef DETACHED_SHINSU_SENJU_H
#define DETACHED_SHINSU_SENJU_H

#include "../../thread_pool/shinsu_senju/ss_pool.h"
#include <stdbool.h>

typedef struct shinsu_senju_data_t
{
    int maximum_workers;
    pthread_t th;
    shinsu_senju_pool_t *pool;
    bool *active;

} shinsu_senju_data_t;

shinsu_senju_data_t *create_shinsu_senju_data(int workers);

int detached_start_shinsu_senju(shinsu_senju_data_t *fshinsu_senju);
int detached_stop_shinsu_senju(shinsu_senju_data_t *fshinsu_senju);
int detached_join_shinsu_senju(shinsu_senju_data_t *fshinsu_senju);
void detached_shinsu_senju_load(shinsu_senju_data_t *ssd,int key,void *(fun)(void *), void *arg);

#endif
