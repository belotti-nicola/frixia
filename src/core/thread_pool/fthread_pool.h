#include "../fqueue/frixia_queue.h"


typedef struct thread_pool
{
    pthread_t *th;
    thread_safe_queue_t *q;
      
} thread_pool_t;


thread_pool_t* create_thread_pool(int n, void *(*dispatcher_fun)(void *));