#ifndef CRONO_H
#define CRONO_H

#include "../../utils/datastructures/threadsafe_timer_wheel/ts_timer_wheel.h"

typedef struct crono
{
    threadsafe_simple_timer_wheel_t tw;
    pthread_t th;

} crono_t;

void crono_add_oneshot_timer(crono_t crono,int delay,void (*fun)(void *),void *arg);
void crono_add_periodic_timer(crono_t crono,int delay,int interval,void (*fun)(void *),void *arg);

crono_t crono_create(int tick_duration);

#endif