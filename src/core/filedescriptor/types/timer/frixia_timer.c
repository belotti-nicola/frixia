#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "frixia_timer.h"

int start_timer_listening(int delay, int interval)
{
    int tfd, epfd;
    struct itimerspec timer_spec;
    struct epoll_event ev;

    tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd == -1)
    {
        return ERR_TIMER_CREATE;
    }

    timer_spec.it_value.tv_sec     = delay;  
    timer_spec.it_value.tv_nsec    = 0; //NOT SIGNIFICATIVE
    timer_spec.it_interval.tv_sec  = interval;
    timer_spec.it_interval.tv_nsec = 0; //NOT SIGNIFICATIVE

    if (timerfd_settime(tfd, 0, &timer_spec, NULL) == -1) {
        return ERR_TIMERFD_SETTIME;
    }

    return tfd;

}
int stop_timer_listening(int fd)
{
    int rc = close(fd);
    if(rc != 0)
    {
        return ERR_CLOSE;
    }
    return TIMER_OK;
}
