#include "../../src/utils/datastructures/simple_timer_wheel/stw_timer_wheel.h"
#include <stdlib.h>

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

    simple_timer_wheel_t tw = simple_timer_wheel_create(1);
    simple_timer_wheel_add_oneshot_timer( &tw,2,increment_counter,&c1);
    simple_timer_wheel_add_periodic_timer(&tw,3,3,increment_counter,&c2);
    simple_timer_wheel_add_oneshot_timer(&tw,33,increment_counter,&c3);
    
    for(int i=0;i<100;i++)
    {
        simple_timer_wheel_tick(&tw);
    }

    if( c1 != 1 || c2 != 32 || c3 != 1)
    {
        printf("ERROR :: C1:%d C2:%d C3:%d\n",c1,c2,c3);
        return 1;
    }

    return 0;
}