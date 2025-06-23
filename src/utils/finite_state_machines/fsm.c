#include "fsm.h"

#include <stdio.h>

void sfsm_add_transition(simple_fsm_t *fsm,int state,int event, int new_state)
{
    if( state >= NUM_STATES )
    {
        printf("ERROR sfsm_add_transition state >= NUM_STATES\n");
        return;
    }

    if( event >= NUM_EVENTS )
    {
        printf("ERROR sfsm_add_transition event >= NUM_EVENTS\n");
        return;
    }

    int index = event * NUM_STATES + state;
    fsm->transitions[index] = new_state;

    return;    
}
void sfsm_step(simple_fsm_t *fsm,int event)
{
    if( event >= NUM_EVENTS || event < 0 )
    {
        printf("WRN sfsm_add_transition event >= NUM_EVENTS\n (%d >= %d)\n",event,NUM_EVENTS);
        return;
    }
    
    int curr_state = fsm->curr_state;
    int index = event * NUM_STATES + curr_state;


    int new_state = fsm->transitions[index];
    if( new_state == -1 )
    {
        return;
    }

    fsm->curr_state = new_state;

    return;
}

simple_fsm_t sfsm_create()
{
    simple_fsm_t fsm = {0};
    for(int i=0;i<NUM_EVENTS*NUM_STATES;i++)
    {
        fsm.transitions[i] = -1;
    }

    return fsm;
}
