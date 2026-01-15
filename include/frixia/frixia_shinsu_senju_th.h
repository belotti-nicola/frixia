
#ifndef FRIXIA_SHINSU_SENJU_TH_H
#define FRIXIA_SHINSU_SENJU_TH_H

#include <pthread.h>
#include <stdbool.h>

typedef struct shinsu_senju_pool shinsu_senju_pool_t;

typedef struct shinsu_senju_data_t
{
    int maximum_workers;
    pthread_t th;
    bool *active;
    shinsu_senju_pool_t *pool;
    
    void *ctx;

} shinsu_senju_data_t;

shinsu_senju_data_t *create_shinsu_senju_data(int workers, void *ctx);
void destroy_shinsu_senju_data(shinsu_senju_data_t *ssd);

int  detached_shinsu_senju_start(shinsu_senju_data_t *fshinsu_senju);
int  detached_shinsu_senju_stop(shinsu_senju_data_t *fshinsu_senju);
int  detached_shinsu_senju_join(shinsu_senju_data_t *fshinsu_senju);
void detached_shinsu_senju_load(shinsu_senju_data_t *ssd,int key,void *(fun)(void *), void *arg);
void detached_shinsu_senju_push(shinsu_senju_data_t *ssd, int key, void *event);


#endif
