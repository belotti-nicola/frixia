#ifndef STW_TIMER_WHEEL_H
#define STW_TIMER_WHEEL_H

#include "stw_common.h"
#include "stw_timer_wheel_slot.h"

typedef struct simple_timer_wheel
{
    int                 tick_duration;
    int                 current_index;
    int                 slots_size;
    simple_wheel_slot_t slots[TIMER_WHEEL_SLOT_SIZE];

} simple_timer_wheel_t;

void simple_timer_wheel_add_oneshot_timer(simple_timer_wheel_t *tw, int delay, void (*fun)(void *), void *arg);
void simple_timer_wheel_add_periodic_timer(simple_timer_wheel_t *tw, int delay, int interval, void (*fun)(void *), void *arg);
void simple_timer_wheel_tick(simple_timer_wheel_t *tw);

simple_timer_wheel_t simple_timer_wheel_create(int tick_duration);

#endif