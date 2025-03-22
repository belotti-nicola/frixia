#ifndef SIMPLE_TIMER_WHEEL_SLOT_H
#define SIMPLE_TIMER_WHEEL_SLOT_H

#include "stw_common.h"
#include "stw_timer.h"
#include "stw_timer_wheel.h"

typedef struct simple_wheel_slot
{
    int current_size;
    simple_timer_t timers[TIMERS_PER_SLOT_NUMBER];

} simple_wheel_slot_t;

void simple_wheel_slot_add_timer(simple_wheel_slot_t *slot,
                                 int delay,
                                 int period,
                                 TIMER_TYPE_T type,
                                 int remaining_rounds,
                                 void (*fun)(void *),
                                 void *arg);

void simple_wheel_slot_remove_timers(simple_wheel_slot_t *slot, simple_timer_wheel_t *tw);

#endif
