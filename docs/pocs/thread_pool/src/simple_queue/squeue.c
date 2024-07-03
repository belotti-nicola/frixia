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
    qptr->size = 0;
    pthread_mutex_t *mutex = malloc(sizeof(*mutex));
    if(mutex == NULL)
    {
        exit(EXIT_FAILURE);
    }

    pthread_cond_t *cond = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(cond,NULL);


    qptr->mutex = mutex;
    qptr->empty = cond;
    return qptr;
}
int pop_q(thread_safe_queue_t* q)
{
    pthread_mutex_lock(q->mutex);
    while( q->size == 0 ) 
    {
        pthread_cond_wait(q->empty,q->mutex);
    }
    if(q->first == NULL)
    {
        pthread_mutex_unlock(q->mutex);
        return -1;
    }

    q_elem_t *pop_this = q->first;
    q->first = pop_this->next;
    int retVal = pop_this->val;
    free(pop_this);
    q->size -= 1;
    pthread_mutex_unlock(q->mutex);
    return retVal;
}
void push_q(thread_safe_queue_t* q,int v)
{
    q_elem_t *el = create_el(v);
    if(el == NULL)
    {
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(q->mutex);
    if(q->first == NULL)
    {
        pthread_cond_broadcast(q->empty);
        q->first = el;
        q->last  = el;
        pthread_mutex_unlock(q->mutex);
        return;
    }

    q->last->next = el;
    q->last = el;
    q->size += 1;
    pthread_mutex_unlock(q->mutex);
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

void destroy_q(thread_safe_queue_t *q)
{
   free(q->mutex);
   free(q);
   q = NULL; 
}
