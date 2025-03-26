#ifndef CRONO_H
#define CRONO_H

#include "../../utils/datastructures/threadsafe_timer_wheel/ts_timer_wheel.h"
#include <stdbool.h>

typedef struct crono
{
    threadsafe_simple_timer_wheel_t *tw;
    pthread_t th;
    bool keep_looping;

} crono_t;

void crono_add_oneshot_timer(crono_t *crono,int delay,void (*fun)(void *),void *arg);
void crono_add_periodic_timer(crono_t *crono,int delay,int interval,void (*fun)(void *),void *arg);
void crono_tick(crono_t *crono);

void crono_stop(crono_t *c);

crono_t crono_create(threadsafe_simple_timer_wheel_t *tw);

#endif