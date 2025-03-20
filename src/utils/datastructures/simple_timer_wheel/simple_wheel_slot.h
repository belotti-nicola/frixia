#include "simple_timer_wheel_common.h"
#include "simple_timer.h"

typedef struct simple_wheel_slot
{
    int            current_size;
    simple_timer_t timers[TIMERS_PER_SLOT_NUMBER];

} simple_wheel_slot_t;

void simple_wheel_slot_add_timer(simple_wheel_slot_t *slot, void (*fun)(void *),void *arg);
void simple_wheel_slot_remove_timers(simple_wheel_slot_t *slot);
