
#include "shinsu_senju_loop_function.h"

#include <frixia/frixia_shinsu_senju_th.h>
#include <internal/ss_pool.h>

void *shinsu_senju_loop_function(void *arg)
{
    shinsu_senju_data_t *ctx = (shinsu_senju_data_t *)arg;
    shinsu_senju_pool_t *ssp = ctx->pool;

    bool *keep_looping = ctx->active;
    while ( *keep_looping )
    {
        ss_join(ssp);
        printf("Empty shinsu_senju\n");
    }

    return NULL;
}