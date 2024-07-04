#include <stdlib.h>
#include <stdio.h>
#include "frixia_queue.h"

thread_safe_queue_t *create_q()
{
  thread_safe_queue_t *qptr = malloc(sizeof(thread_safe_queue_t));
  if (qptr == NULL)
  {
    printf("thread_safe_queue_t* pointer is null!\n");
    exit(EXIT_FAILURE);
  }
  qptr->size = 0;
  pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
  if (mutex == NULL)
  {
    printf("thread_safe_queue_t* mutex is null!\n");
    exit(EXIT_FAILURE);
  }
  pthread_mutex_init(mutex,NULL);

  pthread_cond_t *cond = malloc(sizeof(pthread_cond_t));
  if (cond == NULL)
  {
    printf("thread_safe_queue_t* cond is null!\n");
    exit(EXIT_FAILURE);
  }
  pthread_cond_init(cond, NULL);

  qptr->mutex = mutex;
  qptr->empty = cond;
  return qptr;
}
void* pop_q(thread_safe_queue_t *q)
{
  pthread_mutex_lock(q->mutex);
  while (q->size == 0)
  {
    pthread_cond_wait(q->empty, q->mutex);
  }
  if (q->first == NULL)
  {
    pthread_mutex_unlock(q->mutex);
    return NULL;
  }

  q_elem_t *pop_this = q->first;
  q->first = pop_this->next;
  void* retVal = pop_this->val;
  free(pop_this);
  q->size -= 1;
  pthread_mutex_unlock(q->mutex);
  return retVal;
}
void push_q(thread_safe_queue_t *q, void *v)
{
  q_elem_t *el = create_el(v);
  if (el == NULL)
  {
    printf("push_q element is null!\n");
    exit(EXIT_FAILURE);
  }

  pthread_mutex_lock(q->mutex);
  if (q->first == NULL)
  {
    pthread_cond_broadcast(q->empty);
    q->first = el;
    q->last = el;
    q->size += 1;
    pthread_mutex_unlock(q->mutex);
    return;
  }

  q->last->next = el;
  q->last = el;
  q->size += 1;
  pthread_mutex_unlock(q->mutex);
  return;
}

q_elem_t *create_el(void *p)
{
  q_elem_t *ptr = malloc(sizeof(q_elem_t));
  if (ptr == NULL)
  {
    printf("q_elem_t* pointer is null\n");
    return NULL;
  }

  ptr->val  = p;
  ptr->next = NULL;
  return ptr;
}

void destroy_q(thread_safe_queue_t *q)
{
  free(q->mutex);
  free(q->empty);
  free(q);
  q = NULL;
}
