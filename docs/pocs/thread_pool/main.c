#include "src/simple_queue/squeue.h"
#include "src/simple_thread_pool/sthread_pool.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h> 

#define WORKERS 20
#define TASKS   20

void *work(void *arg)
{
  thread_safe_queue_t *casted_arg = (thread_safe_queue_t*)arg;

  while( true )
  {
    int t = rand() % 5;
    sleep(t);
    int popped_el = pop_q(casted_arg);
    printf("popped:%d (%d)\n",popped_el,casted_arg->size);
  }
  printf("Thread ended\n");
}


int main()
{
  thread_safe_queue_t* q = create_q();
  for(int i=0;i<TASKS;i++)
  {
    push_q(q,i);
  }
  
  thread_pool_t *tp = thread_pool_create(WORKERS,&work,q);
  sleep(5);
  printf("Sleep\n");
  thread_pool_add_job(tp,1);
  thread_pool_add_job(tp,2);
  thread_pool_add_job(tp,3);
  thread_pool_add_job(tp,4);
  sleep(5);

  return 0;
}