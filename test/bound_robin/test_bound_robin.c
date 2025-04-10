#include "../../src/core/thread_pool/bound_robin/bound_robin.h"
#include "../../src/core/thread_pool/bound_robin/bound_robin_event.h"
#include <stdatomic.h> 


void *test_main_function(void *arg)
{
    atomic_int *value_ptr = (atomic_int *)arg;

    int value = atomic_load(value_ptr);
    value++;
    atomic_store(value_ptr,value);
}

int main()
{
    atomic_int test_value = 0;
    atomic_init(&test_value,0);
    bound_robin_event_t test = bound_robin_create_event(
        test_main_function,
        (void *)&test_value
    );

    bound_robin_t br; 
    bound_robin_create(&br,NULL,NULL,test_main_function,test_value);
    bound_robin_add_task(&br,&test);
    bound_robin_add_task(&br,&test);
    bound_robin_add_task(&br,&test);
    bound_robin_broadcast_task(&br,&test);
    sleep(1);
    bound_robin_wait(&br);

    int result = atomic_load(&test_value);
    if( result != 8 )
    {
        printf("Error::test_bound_robin result: %d != 8",result);
        return 1;
    }

    return 0;

}