#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h> 

void *thread_loop(void *arg)
{
    atomic_bool *keep_looping = arg;
    while (atomic_load(keep_looping))
    {
        printf("Executed an iteration.\n");
        sleep(1);
    }
    printf("Thread termination.\n");
    return NULL;
}

int main()
{
    atomic_bool b = true;
    pthread_t t;
    pthread_create(&t, NULL, thread_loop, &b);
    sleep(5);
    atomic_store(&b, false);
    pthread_join(t, NULL);
    return 0;
}