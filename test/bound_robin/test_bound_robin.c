#include "../../src/core/thread_pool/bound_robin/bound_robin.h"
#include <stdatomic.h> 


void test_main_function(void *arg)
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

    bound_robin_t br; 
    bound_robin_create(&br,NULL,NULL,test_main_function,NULL);
    bound_robin_add_task(&br,NULL);
    bound_robin_add_task(&br,NULL);

    if( test_value != 2 )
    {
        return 1;
    }

    return 0;

}