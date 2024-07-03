#ifndef SQUEUE_H
#define SQUEUE_H

#include <stdio.h>
#include <pthread.h>

typedef struct q_elem 
{
    int            val;
    struct q_elem *next;
} q_elem_t;

typedef struct thread_safe_queue 
{
    size_t           size;
    q_elem_t*        first;
    q_elem_t*        last;

    pthread_mutex_t *mutex;

} thread_safe_queue_t;

thread_safe_queue_t* create_q();
q_elem_t* create_el(int v);
int pop_q(thread_safe_queue_t* q);
void push_q(thread_safe_queue_t* q,int v);
void destroy_q(thread_safe_queue_t *t);

#endif