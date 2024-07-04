#include "../fqueue/frixia_queue.h"


typedef struct thread_pool
{
    pthread_t *th;
    thread_safe_queue_t *q;
      
} thread_pool_t;


thread_pool_t* create_thread_pool(int n, void *(*dispatcher_fun)(void *));
void thread_pool_add_job(thread_pool_t *t, void* j);
void thread_pool_join(thread_pool_t *t);