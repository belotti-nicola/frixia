#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h> 

void *thread_loop(bool *keep_looping)
{
    while(*keep_looping)
    {
        printf("Executed an iteration.\n");
        sleep(1);
    }
    printf("Thread termination.\n");
}

int main()
{
    bool b = true;
    pthread_t t;
    pthread_create(&t,NULL,thread_loop,&b);
    sleep(5);
    atomic_store(&b,0);
    pthread_join(t,NULL);
    return 0;
}