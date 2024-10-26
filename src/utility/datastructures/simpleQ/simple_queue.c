#include <stdlib.h>
#include <stdio.h>
#include "simple_queue.h"

simple_queue_t *create_q()
{
  simple_queue_t *qptr = malloc(sizeof(simple_queue_t));
  if (qptr == NULL)
  {
    printf("simple_queue_t* pointer is null!\n");
    exit(EXIT_FAILURE);
  }
  qptr->size = 0;

  return qptr;
}
void* pop_simple_queue(simple_queue_t *q)
{
  queue_elem_t *pop_this = q->first;
  q->first = pop_this->next;
  void* retVal = pop_this->val;
  free(pop_this);
  q->size -= 1;
  return retVal;
}
void push_simple_queue(simple_queue_t *q, void *v)
{
  queue_elem_t *el = create_el(v);
  if (el == NULL)
  {
    printf("push_q element is null!\n");
    exit(EXIT_FAILURE);
  }

  if (q->first == NULL)
  {
    q->first = el;
    q->last = el;
    q->size += 1;
    return;
  }

  q->last->next = el;
  q->last = el;
  q->size += 1;
  return;
}

queue_elem_t *create_el(void *p)
{
  queue_elem_t *ptr = malloc(sizeof(queue_elem_t));
  if (ptr == NULL)
  {
    printf("q_elem_t* pointer is null\n");
    return NULL;
  }

  ptr->val  = p;
  ptr->next = NULL;
  return ptr;
}

void destroy_q(simple_queue_t *q)
{
  free(q);
  q = NULL;
}
