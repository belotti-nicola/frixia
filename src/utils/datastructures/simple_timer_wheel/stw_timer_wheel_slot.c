#include <stdlib.h>
#include <stdio.h>
#include "stw_timer_wheel.h"

#include "stw_timer_wheel_slot.h"

void simple_wheel_slot_add_timer(simple_wheel_slot_t *slot, int delay, int period, TIMER_TYPE_T type,int remaining_rounds, void (*fun)(void *), void *arg)
{
    int index = slot->current_size;
    if (index > TIMERS_PER_SLOT_NUMBER)
    {
        printf("Error simple_wheel_slot_add_timer!!!Already %d timers in this slot\n",TIMERS_PER_SLOT_NUMBER); // TODO: REMOVE + SHIFT
        return;
    }
    slot->timers[index].delay = delay;
    slot->timers[index].interval = period;
    slot->timers[index].type = type; 
    slot->timers[index].remaining_rounds = remaining_rounds;
    slot->timers[index].callback = fun;
    slot->timers[index].arg = arg;

    slot->current_size++;
}

void simple_wheel_slot_remove_timers(simple_wheel_slot_t *slot, simple_timer_wheel_t *tw)
{
    int dim = slot->current_size;
    for (int i = 0; i < dim; i++)
    {
        int remaining_rounds = slot->timers[i].remaining_rounds;
        if (remaining_rounds != 0)
        {
            slot->timers[i].remaining_rounds--;
            continue;
        }
        void (*fun)(void *) = slot->timers[i].callback;
        void *arg = slot->timers[i].arg;
        if (fun != NULL && arg != NULL)
        {
            fun(arg);
        }

        TIMER_TYPE_T t = slot->timers->type;
        if (t == TIMER_PERIODIC)
        {
            int interval = slot->timers[i].interval;
            simple_timer_wheel_add_periodic_timer(tw,interval,interval,fun,arg);
        }
        slot->timers[i].callback = NULL;
        slot->timers[i].arg = NULL;
        slot->timers[i].remaining_rounds = 0;
        slot->current_size--;

        printf("Timer expired!\n");
    }
}
