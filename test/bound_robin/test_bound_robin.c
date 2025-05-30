#include "../../src/core/thread_pool/bound_robin/bound_robin.h"
#include "../../src/core/thread_pool/bound_robin/bound_robin_event.h"
#include <stdatomic.h> 


void *test_main_function(void *arg)
{
    atomic_int *value_ptr = (atomic_int *)arg;
    atomic_fetch_add(value_ptr, 1);
}

int main()
{
    atomic_int test_value = 0;
    atomic_init(&test_value,0);

    bound_robin_t br; 
    bound_robin_create(&br);
    bound_robin_add_task_to_all_workers(&br,test_main_function,&test_value);
    bound_robin_add_task_to_one_worker(&br,test_main_function,&test_value);
    bound_robin_add_task_to_all_workers(&br,test_main_function,&test_value);
    bound_robin_add_task_to_all_workers(&br,test_main_function,&test_value);
    bound_robin_wait(&br);

    int result = atomic_load(&test_value);
    if( result != 16 )
    {
        printf("Error::test_bound_robin result: %d != 16\n",result);
        return 1;
    }

    return 0;

}