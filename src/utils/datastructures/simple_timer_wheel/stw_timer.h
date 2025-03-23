#ifndef STW_TIMER_H
#define STW_TIMER_H

#include "stw_common.h"

typedef struct simple_timer
{
    int delay;
    int interval;
    int remaining_rounds;
    TIMER_TYPE_T type;
    void *callback;
    void *arg;

} simple_timer_t;


simple_timer_t simple_timer_wheel_timer_create(int delay,int interval, TIMER_TYPE_T type);


#endif
