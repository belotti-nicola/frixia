#include "src/simple_queue/squeue.h"
#include "src/simple_thread_pool/sthread_pool.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h> 

#define WORKERS 10
#define TASKS 10

void *work(void *arg)
{
  sleep(3);
  thread_safe_queue_t *casted_arg = (thread_safe_queue_t*)arg;
  //TODO:: CHECK EMPTY Q
  while( casted_arg->size > 0)
  {
    int t = rand() % 5;
    sleep(t);
    int popped_el = pop_q(casted_arg);
    printf("popped:%d (%d)\n",popped_el,t);
  }
  printf("Thread ended\n");
}


int main()
{
  thread_pool_t *tp = thread_pool_create(WORKERS,&work);
  for(int i=0;i<TASKS;i++)
  {
    thread_pool_add_job(tp,&i);
  }
  thread_pool_join(tp);
  return 0;
}