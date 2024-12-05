#ifndef THREADSAFE_SIMPLE_QUEUE_H
#define THREADSAFE_SIMPLE_QUEUE_H

#include "../simple_queue/simple_queue.h"
#include "pthread.h"

typedef struct threadsafe_simple_queue 
{
    int                size;
    simple_queue_t    *queue;
    pthread_mutex_t   *mutex;
    pthread_cond_t    *empty;

} threadsafe_simple_queue_t;

threadsafe_simple_queue_t*  create_threadsafe_simple_queue();
void*                       pop_threadsafe_simple_queue(threadsafe_simple_queue_t* q);
void                        push_threadsafe_simple_queue(threadsafe_simple_queue_t* q,void *el);
void                        destroy_threadsafe_simple_queue(threadsafe_simple_queue_t *t);
bool                        simple_threadsafe_queue_is_empty(threadsafe_simple_queue_t *t);

#endif