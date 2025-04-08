#include "../../src/core/thread_pool/bound_robin/bound_robin.h"

void counting_function(void *arg)
{
    int *casted_arg = (int *)arg;
    (*casted_arg)+arg;
}

void test_main_function(void *arg)
{
    
}


int main()
{

    int counter=0;
    
    bound_robin_t test;
    bound_robin_create(&test,counting_function,counter,NULL,NULL);
    bound_robin_wait(&test);

    return 0;

}