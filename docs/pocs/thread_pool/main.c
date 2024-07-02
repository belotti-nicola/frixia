#include "src/simple_queue/squeue.h"
#include "src/simple_thread_pool/sthread_pool.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#define WORKERS 1
#define TASKS 100

void *work(void *p)
{
  int s = rand()%5;
  sleep(s);
  int* popped = (int *)p;
  printf("A %d\n",*popped);
}


int main()
{
  thread_pool_t *tp = thread_pool_create(WORKERS,work);
  sleep(10);
  return 0;
}