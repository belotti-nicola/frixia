#include <stdlib.h>
#include <stdio.h>

#include "stw_timer_wheel.h"

void simple_timer_wheel_add_oneshot_timer(simple_timer_wheel_t *tw, int delay,void (*fun)(void *), void *arg)
{
    int slots_number = tw->slots_size;
    int tick_duration = tw->tick_duration;
    
    int wheel_duration = slots_number * tick_duration;
    int target_slot = (delay / tick_duration + tw->current_index ) % slots_number;
    int target_round = delay / wheel_duration;

    simple_wheel_slot_t *slot = tw->slots;
    simple_wheel_slot_add_timer(slot+target_slot,delay,0,TIMER_ONESHOT,target_round,fun,arg);   

}
void simple_timer_wheel_add_periodic_timer(simple_timer_wheel_t *tw, int delay, int interval, void (*fun)(void *), void *arg)
{
    int slots_number = tw->slots_size;
    int tick_duration = tw->tick_duration;
    
    int wheel_duration = slots_number * tick_duration;
    int target_slot = (delay / tick_duration + tw->current_index ) % slots_number;
    int target_round = delay / wheel_duration;

    simple_wheel_slot_t *slot = tw->slots;
    simple_wheel_slot_add_timer(slot+target_slot,delay,interval,TIMER_PERIODIC,target_round,fun,arg); 
}
void simple_timer_wheel_tick(simple_timer_wheel_t *tw)
{
    int index = tw->current_index;
    int slots_number = tw->slots_size;
    simple_wheel_slot_t *slot = tw->slots+index;
    simple_wheel_slot_remove_timers(slot,tw);   

    tw->current_index = (++tw->current_index ) % slots_number;

}

simple_timer_wheel_t simple_timer_wheel_create(int tick_duration)
{
    simple_timer_t default_timer = simple_timer_wheel_timer_create(0,0,TIMER_ONESHOT,INACTIVE);   
    
    simple_timer_wheel_t tw;
    tw.current_index = 0;
    tw.tick_duration = tick_duration;
    tw.slots_size = TIMER_WHEEL_SLOT_SIZE;
    for(int i=0;i<TIMER_WHEEL_SLOT_SIZE;i++)
    {
        tw.slots[i].size = TIMERS_PER_SLOT_NUMBER;
        for(int j=0;j<TIMERS_PER_SLOT_NUMBER;j++)
        {
            tw.slots[i].timers[j] = default_timer;
        }
    }

    return tw;
}