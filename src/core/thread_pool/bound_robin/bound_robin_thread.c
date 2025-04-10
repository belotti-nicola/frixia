#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bound_robin_event.h"

#include "bound_robin_thread.h"

void bound_robin_thread_main_loop(void *argument)
{
    thread_context_t *ctx = (thread_context_t *)argument;
    threadsafe_simple_queue_t *events = ctx->thread_events;

    void *(*fun)(void *) = ctx->cb_main;
    void   *arg          = ctx->cb_arg;

    atomic_bool *keep_looping = ctx->keep_looping;
    pthread_barrier_wait(ctx->create_barrier);
    while( atomic_load(keep_looping) == true )
    {
        void *event = pop_threadsafe_simple_queue(events);
        if(event == NULL)
        {
            printf("Event null!!!\n");
            continue;
        }
        bound_robin_event_t *ev = (bound_robin_event_t *)event;
        ev->fun(ev->arg);
        printf("BR::Event popped done.\n");
        /*
        cb_arg.event = event;
        cb_arg.client_code = ctx->cb_arg;
        fun((void *)&cb_arg);
        */
    }

    printf("bound_robin_thread_main_loop endend.\n");
}

void bound_robin_thread_stop(thread_context_t *ctx)
{
    atomic_bool *target = ctx->keep_looping;
    atomic_store(target,0);   
}

thread_context_t *bound_robin_create_thread_context()
{
    thread_context_t *ctx = malloc(sizeof(thread_context_t));
    if( ctx == NULL )
    {
        printf("bound_robin_create_thread_context Error!! thread_context_t\n");
        return NULL;
    }

    threadsafe_simple_queue_t *q = malloc(sizeof(threadsafe_simple_queue_t));
    if( ctx == NULL )
    {
        printf("bound_robin_create_thread_context Error threadsafe_simple_queue_t!!\n");
        return NULL;
    }
    ctx->thread_events = q;

    atomic_bool *b = malloc(sizeof(atomic_bool));
    if( ctx == NULL )
    {
        printf("bound_robin_create_thread_context Error keep_looping!!\n");
        return NULL;
    }
    atomic_init(b,1);
    ctx->keep_looping = b;

    return ctx;
}
