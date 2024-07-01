#include "sthread_pool.h"

thread_pool_t *thread_pool_create(int n, void *(*f)(void*), void *arg)
{
    thread_pool_t *tp;

    pthread_t th;
    for(int i=0;i<n;i++)
    {
        pthread_create(&th,NULL,f,arg);
        pthread_detach(th);
    }

    return NULL;
}
