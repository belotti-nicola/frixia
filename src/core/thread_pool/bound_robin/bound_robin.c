#include "bound_robin.h"

void bound_robin_add_task(bound_robin_t *br, void *task)
{
    int index = br->current_index;
    delegate_target_thread_with_policy(br,index,task);
    br->current_index = br->current_index + 1;
}
void bound_robin_broadcast_task(bound_robin_t *br, void *task)
{
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        delegate_target_thread_with_policy(br,i,task);
    }
}

void delegate_target_thread_with_policy(bound_robin_t *br,int index,void *task)
{

}

void bound_robin_create(bound_robin_t *br,
                        void *th_main_fun(void *),void *th_main_arg,
                        void *th_delegate_fun(void *),void *th_delegate_arg)
{
    br->delegate_function = th_delegate_fun;
    br->delegate_argument = th_delegate_arg;
    

    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        int rc = pthread_create(&(br->th[i]),NULL,th_main_fun,th_main_arg);
        if(rc < 0)
        {
            printf("Error pthread_create: %d\n",rc);
            continue;
        }
    }
}

void bound_robin_wait(bound_robin_t *br)
{
    for(int i=0;i<FRIXIA_WORKERS;i++)
    {
        pthread_join(br->th[i],NULL);
    }
}