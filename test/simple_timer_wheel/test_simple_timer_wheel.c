#include "../../src/utils/datastructures/simple_timer_wheel/stw_timer_wheel.h"
#include <stdlib.h>
#include <stdio.h>

typedef void (*GenericFunctionPointer)(); 

void *increment_counter(void *counter)
{
    int *counter_casted = (int *)counter;
    *counter_casted = *counter_casted+1;
}

int main()
{
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;

    GenericFunctionPointer counting_function = (GenericFunctionPointer)increment_counter;

    simple_timer_wheel_t tw = simple_timer_wheel_create(1);
    simple_timer_wheel_add_oneshot_timer( &tw,2,counting_function,&c1);
    simple_timer_wheel_add_periodic_timer(&tw,5,2,counting_function,&c2);
    simple_timer_wheel_add_oneshot_timer(&tw,33,counting_function,&c3);
    
    for(int i=0;i<21;i++)
    {
        simple_timer_wheel_tick(&tw);
    }

    if( c1 != 1 || c2 != 8 || c3 != 0)
    {
        printf("ERROR :: C1:%d!=1 C2:%d!=8 C3:%d!=0\n",c1,c2,c3);
        return 1;
    }

    return 0;
}