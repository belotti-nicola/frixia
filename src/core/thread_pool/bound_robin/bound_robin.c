#include "bound_robin.h"

int bound_robin_add_task(bound_robin_t *br, void *task)
{
    return;
}
int bound_robin_broadcast_task(bound_robin_t *br, void *task)
{
    return;
}

void delegate_target_thread_with_policy(bound_robin_t *br,int index,void *task)
{

}

bound_robin_t bound_robin_create(void *th_fun(void *),void *th_arg,
                                 void *th_delegate_fun(void *),void *th_delegate_arg)
{
    bound_robin_t br;
    pthread_t *thread;

    br.delegate_function = th_delegate_fun;
    br.delegate_argument = th_delegate_arg;

    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        int rc = pthread_create(thread,NULL,th_fun,th_arg);
        if(rc < 0)
        {
            printf("Error pthread_create: %d\n",rc);
            continue;
        }
        br.th[i] = thread;
    }
}