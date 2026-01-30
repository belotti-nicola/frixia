
#include "shinsu_senju_loop_function.h"

#include <frixia/frixia_shinsu_senju_th.h>
#include <internal/ss_pool.h>
#include <frixia/frixia_environment.h>


//TODO CLEAN
#include <signal.h>

void *shinsu_senju_loop_function(void *arg)
{
    shinsu_senju_data_t *ssd = (shinsu_senju_data_t *)arg;
    shinsu_senju_pool_t *ssp = ssd->pool;

    bool *keep_looping = ssd->active;
    while ( *keep_looping )
    {
        ss_join(ssp);
        printf("Empty shinsu_senju\n");
    }

    return NULL;
}