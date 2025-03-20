#include <stdlib.h>
#include <stdio.h>

#include "simple_timer_wheel.h"

typedef struct periodic_context
{
    simple_timer_wheel_t *tw;
    int delay;
    void (*fun)(void *);
    void *arg;

} periodic_context_t;

void *periodic_timer_callback(periodic_context_t *ctx)
{
    if(ctx->fun == NULL )
    {
        return;
    }
    if(ctx->arg == NULL )
    {
        return;
    }

    ctx->fun(ctx->arg);
    simple_timer_wheel_add_oneshot_timer(
        ctx->tw,
        ctx->delay,
        ctx->fun,
        ctx->arg
    );

}


void simple_timer_wheel_add_oneshot_timer(simple_timer_wheel_t *tw, int delay,void (*fun)(void *), void *arg)
{
    int slots = TIMER_WHEEL_SLOT_SIZE;
    int tick_duration = tw->tick_duration;
    
    int wheel_duration = slots * tick_duration;
    int target_slot = (delay / wheel_duration) % TIMER_WHEEL_SLOT_SIZE;
    int target_round = delay / wheel_duration;

    simple_wheel_slot_t *slot = tw->slots;
    simple_wheel_slot_add_timer(slots+target_slot,fun,arg);   
}
void simple_timer_wheel_add_periodic_timer(simple_timer_wheel_t *tw, int delay,void (*fun)(void *), void *arg)
{
    int slots = TIMER_WHEEL_SLOT_SIZE;
    int tick_duration = tw->tick_duration;
    
    int wheel_duration = slots * tick_duration;
    int target_slot = (delay / wheel_duration) % TIMER_WHEEL_SLOT_SIZE;
    int target_round = delay / wheel_duration;

    simple_wheel_slot_t *slot = tw->slots;
    periodic_context_t ctx = {
        .tw = tw,
        .delay = delay,
        .fun = fun,
        .arg = arg
    };
    simple_wheel_slot_add_timer(slots+target_slot,periodic_timer_callback,&ctx); 
}
void simple_timer_wheel_tick(simple_timer_wheel_t *tw)
{
    int index = tw->current_index++;
    simple_wheel_slot_t *slot = tw->slots+index;
    simple_wheel_slot_remove_timers(slot);
    
}