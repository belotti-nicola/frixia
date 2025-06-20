#include "../../src/utils/finite_state_machines/fsm.h"

int main()
{
    simple_fsm_t fsm;
    sfsm_add_transition(&fsm,0,0,1);
    sfsm_add_transition(&fsm,1,1,2);
    sfsm_add_transition(&fsm,2,2,3);

    sfsm_step(&fsm,0);
    if ( fsm.curr_state != 1 )
    {
        return 1;
    }

    sfsm_step(&fsm,1);
    if ( fsm.curr_state != 2 )
    {
        return 1;
    }

    sfsm_step(&fsm,2);
    if ( fsm.curr_state != 3 )
    {
        return 1;
    }
    
    sfsm_step(&fsm,100);
    if ( fsm.curr_state != 3 )
    {
        return 1;
    }
    
    return 0;
}