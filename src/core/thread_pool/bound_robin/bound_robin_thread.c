#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bound_robin_thread.h"

void bound_robin_thread_main_loop(void *argument)
{
    thread_context_t *ctx = (thread_context_t *)argument;
    threadsafe_simple_queue_t *events = ctx->thread_events;

    void *(*fun)(void *) = ctx->cb_main;
    void   *arg          = ctx->cb_arg;

    bool *keep_looping = ctx->keep_looping;
    while( *keep_looping == true )
    {
        printf("BR::Event popping %d\n",*keep_looping);
        void *event = pop_threadsafe_simple_queue(events);
        printf("BR::Event popped %d\n",*keep_looping);
        if(event == NULL)
        {
            sleep(1);
            continue;
        }
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
    bool *target = ctx->keep_looping;
    *target = false;    
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

    bool *b = malloc(sizeof(bool));
    if( ctx == NULL )
    {
        printf("bound_robin_create_thread_context Error keep_looping!!\n");
        return NULL;
    }
    *b = true;
    ctx->keep_looping = b;

    return ctx;
}
