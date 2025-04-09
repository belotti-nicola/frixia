#include <stdbool.h>
#include <unistd.h>

#include "bound_robin_thread.h"

void bound_robin_thread_main_loop(void *argument)
{
    thread_context_t *ctx = (thread_context_t *)argument;

    bool keep_looping = *(ctx->keep_looping);
    threadsafe_simple_queue_t *events = ctx->thread_events;

    void *(*fun)(void *) = ctx->cb_main;
    void   *arg          = ctx->cb_arg;

    callback_arg_t cb_arg;
    while( keep_looping )
    {
        void *event = pop_threadsafe_simple_queue(events);
        if(event == NULL)
        {
            sleep(100);
            continue;
        }
        printf("BR::Event popped\n");
        /*
        cb_arg.event = event;
        cb_arg.client_code = ctx->cb_arg;
        fun((void *)&cb_arg);
        */
    }
}

void bound_robin_thread_stop(thread_context_t *ctx)
{
    ctx->keep_looping = false;
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
    ctx->keep_looping = b;

    return ctx;
}
