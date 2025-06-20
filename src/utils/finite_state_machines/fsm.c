#include "fsm.h"

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
    if( event >= NUM_EVENTS )
    {
        printf("ERROR sfsm_add_transition event >= NUM_EVENTS\n");
        return;
    }

    int state = fsm->curr_state;
    int index = event * NUM_STATES + state;
    int new_state = fsm->transitions[index];

    fsm->curr_state = new_state;
    return;
}
