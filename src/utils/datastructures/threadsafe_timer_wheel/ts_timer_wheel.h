#ifndef THREADSAFE_TIMER_WHEEL_H
#define THREADSAFE_TIMER_WHEEL_H

#include <pthread.h>

#include "../simple_timer_wheel/stw_timer_wheel.h"

typedef struct threadsafe_simple_timer_wheel
{
    pthread_mutex_t        lock;
    simple_timer_wheel_t  _tw;

} threadsafe_simple_timer_wheel_t;

void ts_timer_wheel_add_oneshot_timer(threadsafe_simple_timer_wheel_t *tw, int delay, void (*fun)(void *), void *arg);
void ts_timer_wheel_add_periodic_timer(threadsafe_simple_timer_wheel_t *tw, int delay, int interval, void (*fun)(void *), void *arg);
void ts_timer_wheel_tick(threadsafe_simple_timer_wheel_t *tw);

threadsafe_simple_timer_wheel_t ts_timer_wheel_create(int tick_duration);

#endif