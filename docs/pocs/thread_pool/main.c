#include<stdio.h>
#include<stdlib.h>

#define Q_LENGTH 40

static void foo(int i)
{
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


int main()
{
    simple_queue_t* sq = create_simple_list(Q_LENGTH);
    simple_queue_el_t* sqel = sq->first;
    for(int i=0;i<Q_LENGTH;i++)
    {
        printf("%d\n",sqel->arg);
        sqel = sqel->next;
    }
    return 0;
}