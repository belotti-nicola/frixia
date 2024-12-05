#include <stdlib.h>
#include <pthread.h>
#include "../simple_queue/simple_queue.h"


#include "threadsafe_simple_queue.h"

threadsafe_simple_queue_t* create_threadsafe_simple_queue()
{  
    threadsafe_simple_queue_t *q = malloc(sizeof(threadsafe_simple_queue_t));
    if( q == NULL)
    {
        printf("ERROR CREATING threadsafe_simple_queue_t\n");
        return NULL;
    }
    simple_queue_t *squeu = create_simple_queue();
    if( squeu == NULL )
    {
        printf("ERROR CREATING simple_queue\n");
        return NULL;
    }
    q->queue = squeu;
    
    q->size = 0;
    
    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex, NULL);
    q->mutex = mutex;

    pthread_cond_t *m_empty = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(m_empty,NULL);
    q->empty = m_empty;

    return q;
}

void* pop_threadsafe_simple_queue(threadsafe_simple_queue_t* q)
{
    void *el;
    pthread_mutex_lock(q->mutex);
    simple_queue_t *concrete_q = q->queue;
    while( concrete_q->size == 0)
    {
        pthread_cond_wait(q->empty,q->mutex);
    }
    
    el = pop_simple_queue(concrete_q);
    q->size -= 1;
    pthread_mutex_unlock(q->mutex);
    return el;
}
void push_threadsafe_simple_queue(threadsafe_simple_queue_t* q,void *el)
{
    pthread_mutex_lock(q->mutex);
    simple_queue_t *concrete_q = q->queue;
    push_simple_queue(concrete_q,el);
    q->size++;
    if(q->queue->size == 1)
    {
        pthread_cond_signal(q->empty);
    }
    pthread_mutex_unlock(q->mutex);
}
void destroy_threadsafe_simple_queue(threadsafe_simple_queue_t *t)
{
    destroy_simple_queue(t->queue);
    free(t);
}
bool simple_threadsafe_queue_is_empty(threadsafe_simple_queue_t *t)
{
    return t->queue->size == 0;
}