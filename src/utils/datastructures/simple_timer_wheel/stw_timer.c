#include "stw_common.h"

#include "stw_timer.h"

simple_timer_t simple_timer_wheel_timer_create(int delay,int interval, TIMER_TYPE_T type)
{
    simple_timer_t ret;
    ret.delay            = delay;
    ret.interval         = interval;
    ret.remaining_rounds = 0;
    ret.type             = type;

    return ret;
}