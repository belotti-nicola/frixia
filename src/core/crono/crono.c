#include "../../utils/datastructures/threadsafe_timer_wheel/ts_timer_wheel.h"

#include "crono.h"

void crono_add_oneshot_timer(crono_t crono,int delay,void (*fun)(void *),void *arg)
{
    threadsafe_simple_timer_wheel_t *tw = &(crono.tw);
    ts_timer_wheel_add_oneshot_timer(tw,delay,fun,arg);
}
void crono_add_periodic_timer(crono_t crono,int delay,int interval,void (*fun)(void *),void *arg)
{
    threadsafe_simple_timer_wheel_t *tw = &(crono.tw);
    ts_timer_wheel_add_periodic_timer(tw,delay,interval,fun,arg);
}

crono_t crono_create(int tick_duration)
{
    crono_t ret;
    ret.tw = ts_timer_wheel_create(tick_duration);
    
    return ret;
}