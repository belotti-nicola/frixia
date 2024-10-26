#ifndef SIMPLE_QUEUE_H
#define SIMPLE_QUEUE_H

#include <stdio.h>

typedef struct queue_elem 
{
    void              *val;
    struct queue_elem     *next;
} queue_elem_t;

typedef struct simple_queue 
{
    int                    size;
    queue_elem_t          *first;
    queue_elem_t          *last;

} simple_queue_t;

simple_queue_t*        create_simple_queue();
queue_elem_t*          create_element(void *el);
void*                  pop_simple_queue(simple_queue_t* q);
void                   push_simple_queue(simple_queue_t* q,void *el);
void                   destroy_simple_queue(simple_queue_t *t);

#endif