#ifndef FSM_H_H
#define FSM_H_H

#include "fsm_common.h"

typedef struct simple_fsm_t
{
    int curr_state;
    int transitions[NUM_EVENTS * NUM_STATES];

} simple_fsm_t;

void sfsm_add_transition(simple_fsm_t *fsm,int state,int event, int new_state);
void sfsm_step(simple_fsm_t *fsm, int event);

simple_fsm_t sfsm_create();

#endif