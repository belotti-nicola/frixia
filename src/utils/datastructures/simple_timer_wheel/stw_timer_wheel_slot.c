#include <stdlib.h>
#include <stdio.h>
#include "stw_timer_wheel.h"

#include "stw_timer_wheel_slot.h"

void simple_wheel_slot_add_timer(simple_wheel_slot_t *slot, int delay, int period, TIMER_TYPE_T type,int remaining_rounds, void (*fun)(void *), void *arg)
{
    int index=0;
    for( ;index < TIMERS_PER_SLOT_NUMBER; index++)
    {
        if( slot->timers[index].status == INACTIVE )
        {
            break;
        }
    }
    
    if (index == TIMERS_PER_SLOT_NUMBER)
    {
        // TODO: REMOVE + SHIFT
        printf("Error simple_wheel_slot_add_timer!!!Already %d timers in this slot\n",TIMERS_PER_SLOT_NUMBER); 
        return;
    }
    slot->timers[index].delay = delay;
    slot->timers[index].interval = period;
    slot->timers[index].type = type; 
    slot->timers[index].remaining_rounds = remaining_rounds;
    slot->timers[index].callback = fun;
    slot->timers[index].arg = arg;
    slot->timers[index].status = ACTIVE;

}

void simple_wheel_slot_remove_timers(simple_wheel_slot_t *slot, simple_timer_wheel_t *tw)
{
    int dim = TIMERS_PER_SLOT_NUMBER;
    for (int i = 0; i < dim; i++)
    {
        if( slot->timers[i].status == INACTIVE )
        {
            continue;
        }
        
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

        slot->timers[i].callback = NULL;
        slot->timers[i].arg = NULL;
        slot->timers[i].remaining_rounds = 0;
        slot->timers[i].status = INACTIVE;

        TIMER_TYPE_T t = slot->timers[i].type;
        if (t == TIMER_PERIODIC)
        {
            int interval = slot->timers[i].interval;
            simple_timer_wheel_add_periodic_timer(tw,interval,interval,fun,arg);
        }

        printf("Timer expired! %d\n",t);
    }
}
