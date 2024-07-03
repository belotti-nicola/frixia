#include "src/simple_queue/squeue.h"
#include "src/simple_thread_pool/sthread_pool.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#define WORKERS 1
#define TASKS 100

void *work(void *arg)
{
  int s = 1 + rand()%4;
  sleep(s);
  int* casted_arg = (int*)arg;
  printf("END %d\n",*casted_arg);
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