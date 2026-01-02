#include "../../src/core/thread_pool/shinsu_senju/detached_shinsu_senju.h"
#include "../../src/core/thread_pool/shinsu_senju/ss_wrapper_function.h"
#include <stdio.h>
#include "unistd.h"


void *test_main_function(void *arg)
{
    ss_worker_ctx_t *ctx = (ss_worker_ctx_t *)arg;

    int *my_super_integer = (int *)ctx->callback.auxiliary;
    *my_super_integer = *my_super_integer + 1;
   
    return NULL;
}

int main()
{
    int a = 0;
    int b = 0;
    int c = 0;

    frixia_event_t e = {
        .fd = 1,
        .events_maks = 0L
    };

    shinsu_senju_data_t *ssd = create_shinsu_senju_data(10,NULL);
    detached_shinsu_senju_start(ssd);
    detached_shinsu_senju_load(ssd,0,test_main_function,&a);
    detached_shinsu_senju_load(ssd,1,test_main_function,&b);
    detached_shinsu_senju_load(ssd,5,test_main_function,&b);
    detached_shinsu_senju_push(ssd,0,&e);
    detached_shinsu_senju_push(ssd,1,&e);
    sleep(0.5);
    detached_shinsu_senju_push(ssd,1,&e);
    sleep(0.5);
    detached_shinsu_senju_stop(ssd);
    detached_shinsu_senju_join(ssd);

    if ( a != 1 )
    {
        printf("Error: a!=1 (actually is: %d)\n",a);
        return 1;
    }
    if ( b != 2 )
    {
        printf("Error: b!=2 (actually is: %d)\n",b);
        return 1;
    }
    if ( c != 0 )
    {
        printf("Error: c!=0 (actually is: %d)\n",c);
        return 1;
    }

    return 0;
}