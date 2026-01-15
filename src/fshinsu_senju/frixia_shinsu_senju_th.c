#include <frixia/frixia_shinsu_senju_th.h>

shinsu_senju_data_t *create_shinsu_senju_data(int workers, void *ctx)
{
    return NULL;
}
void destroy_shinsu_senju_data(shinsu_senju_data_t *ssd)
{}

int  detached_shinsu_senju_start(shinsu_senju_data_t *fshinsu_senju)
{return 0;}
int  detached_shinsu_senju_stop(shinsu_senju_data_t *fshinsu_senju)
{return 0;}
int  detached_shinsu_senju_join(shinsu_senju_data_t *fshinsu_senju)
{return 0;}
void detached_shinsu_senju_load(shinsu_senju_data_t *ssd,int key,void *(fun)(void *), void *arg)
{}
void detached_shinsu_senju_push(shinsu_senju_data_t *ssd, int key, void *event)
{}