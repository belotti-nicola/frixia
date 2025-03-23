#include "../simple_timer_wheel/stw_timer_wheel.h"
#include "ts_timer_wheel.h"

void ts_timer_wheel_add_oneshot_timer(threadsafe_simple_timer_wheel_t *tw, int delay, void (*fun)(void *), void *arg)
{
    simple_timer_wheel_t *concrete = &(tw->_tw);
    pthread_mutex_t mutex = tw->lock;
    pthread_mutex_lock(&mutex);
    simple_timer_wheel_add_oneshot_timer(concrete,delay,fun,arg);
    pthread_mutex_unlock(&mutex);
}
void ts_timer_wheel_add_periodic_timer(threadsafe_simple_timer_wheel_t *tw, int delay, int interval, void (*fun)(void *), void *arg)
{
    simple_timer_wheel_t *concrete = &(tw->_tw);
    pthread_mutex_t mutex = tw->lock;
    pthread_mutex_lock(&mutex);
    simple_timer_wheel_add_periodic_timer(concrete,delay,interval,fun,arg);
    pthread_mutex_unlock(&mutex);
}
void ts_timer_wheel_tick(threadsafe_simple_timer_wheel_t *tw)
{
    simple_timer_wheel_t *concrete = &(tw->_tw);
    pthread_mutex_t mutex = tw->lock;
    pthread_mutex_lock(&mutex);
    simple_timer_wheel_tick(concrete);
    pthread_mutex_unlock(&mutex);
}

threadsafe_simple_timer_wheel_t ts_timer_wheel_create(int tick_duration)
{
    threadsafe_simple_timer_wheel_t ret;
    ret._tw = simple_timer_wheel_create(tick_duration);
    pthread_mutex_init(&(ret.lock), NULL);
    return ret;
}