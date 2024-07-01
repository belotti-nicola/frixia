#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define Q_ELEMENTS 40
#define THREAD_POOL_DIMENSION 5

static void foo(int i)
{
    int r = rand() % 5;
    sleep(r);
    printf("%d\n",i);
}

typedef struct simple_queue_el
{
    int arg;
    void* foo;
    struct simple_queue_el* next;
} simple_queue_el_t;

typedef struct simple_queue
{
    simple_queue_el_t *first;
    simple_queue_el_t *last;
    size_t qdimension;

} simple_queue_t;

simple_queue_t* create_simple_list(size_t n)
{
    simple_queue_t* qptr = calloc(sizeof(simple_queue_t),1);
    if(qptr == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    simple_queue_el_t* el = malloc(sizeof(simple_queue_el_t));
    if(el == NULL)
    {
        exit(EXIT_FAILURE);
    }
    el->arg = 0;
    el->foo = &foo;
    
    simple_queue_el_t* prev = el;
    simple_queue_el_t* cur;
    for(int i=0;i<n-1;i++)
    {
        cur = malloc(sizeof(simple_queue_el_t));
        if(cur == NULL)
        {
            exit(EXIT_FAILURE);
        }
        cur->arg = i+1;
        cur->foo = &foo;
        prev->next = cur;
        prev = cur;
    }
    qptr->first = el;
    qptr->last = cur;

    return qptr;

}

typedef struct thread_pool
{
    size_t dim;

} thread_pool_t;

void create_thread_pool(int n)
{
    pthread_t* thread;
    for (int i=0; i<n; i++) 
    {
        pthread_create( &thread, NULL, foo, );
    }
}


int main()
{
    simple_queue_t* sq = create_simple_list(Q_ELEMENTS);
    simple_queue_el_t* sqel = sq->first;
    for(int i=0;i<Q_ELEMENTS;i++)
    {
        printf("%d\n",sqel->arg);
        sqel = sqel->next;
    }

    create_thread_pool(THREAD_POOL_DIMENSION);

    return 0;
}