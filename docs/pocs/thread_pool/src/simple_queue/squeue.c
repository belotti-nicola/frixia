#include <stdlib.h>
#include <stdio.h>
#include "squeue.h"

thread_safe_queue_t* create_q()
{
    thread_safe_queue_t* qptr = malloc(sizeof(thread_safe_queue_t));
    if(qptr == NULL)
    {
        exit(EXIT_FAILURE);
    }
}
int pop_q(thread_safe_queue_t* q)
{
    if(q->first == NULL)
    {
        return -1;
    }

    q_elem_t *pop_this = q->first;
    q->first = pop_this->next;
    int retVal = pop_this->val;
    free(pop_this);
    return retVal;
}
void push_q(thread_safe_queue_t* q,int v)
{
    q_elem_t *el = create_el(v);
    if(q->first == NULL)
    {
        q->first = el;
        q->last  = el;
        return;
    }

    q->last->next = el;
    q->last = el;

    return;
}

q_elem_t* create_el(int v)
{
    q_elem_t* ptr = malloc(sizeof(q_elem_t));
    if(ptr == NULL)
    {
        return NULL;
    }

    ptr->val  = v;
    ptr->next = NULL;
    return ptr;
}
