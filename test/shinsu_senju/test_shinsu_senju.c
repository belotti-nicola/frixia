#include "../../src/core/thread_pool/shinsu_senju/detached_shinsu_senju.h"
#include <stdio.h>


void *test_main_function(void *arg)
{
    return NULL;
}

int main()
{
    shinsu_senju_data_t *ssd = create_shinsu_senju_data(10);
    detached_start_shinsu_senju(ssd);
    detached_shinsu_senju_load(ssd,0,test_main_function,NULL);
    detached_shinsu_senju_load(ssd,1,test_main_function,NULL);
    detached_stop_shinsu_senju(ssd);
    detached_join_shinsu_senju(ssd);

    return 0;
}