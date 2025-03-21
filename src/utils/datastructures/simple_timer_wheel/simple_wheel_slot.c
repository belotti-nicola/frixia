#include <stdlib.h>
#include "simple_wheel_slot.h"

void simple_wheel_slot_add_timer(simple_wheel_slot_t *slot, void (*fun)(void *),void *arg)
{
    int index = slot->current_size;
    if( index > TIMERS_PER_SLOT_NUMBER)
    {
        printf("Error!!!\n"); //TODO: REMOVE + SHIFT
    }

    slot->timers[index].callback = fun;
    slot->timers[index].arg      = arg;
    
    slot->current_size++;

}
void simple_wheel_slot_remove_timers(simple_wheel_slot_t *slot)
{
    int dim = slot->current_size;
    printf("Slot size: %d\n",dim);
    for(int i=0;i<dim;i++)
    {
        void (*fun)(void *) = slot->timers[i].callback;
        void *arg           = slot->timers[i].arg;
        if( fun == NULL)
        {
            printf("simple_wheel_slot_remove_timers fun is NULL\n");
            return;
        }
        if( arg == NULL)
        {
            printf("simple_wheel_slot_remove_timers arg is NULL\n");
            return;
        }
        fun(arg);
        slot->timers[i].callback = NULL;
        slot->timers[i].arg      = NULL;
    }

    slot->current_size = 0;
}