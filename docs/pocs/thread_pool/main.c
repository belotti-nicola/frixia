#include "src/simple_queue/squeue.h"
#include <stdio.h>

int main()
{
  thread_safe_queue_t *q = create_q();
  push_q(q,1);
  push_q(q,2);
  push_q(q,3);
  push_q(q,4);
  push_q(q,5);
  push_q(q,6);
  
  int popped_el = -1;
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));
  printf("%d\n",pop_q(q));


  return 0;
}