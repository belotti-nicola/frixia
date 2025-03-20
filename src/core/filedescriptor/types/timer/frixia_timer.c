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
    printf("Starting listening timer: %d %d\n",delay,interval);
    int tfd, epfd;
    struct itimerspec timer_spec;
    struct epoll_event ev;

    tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfd == -1)
    {
        printf("ERROR start_timer_listening::%d\n",errno);
        return ERR_TIMER_CREATE;
    }

    bzero(&timer_spec, sizeof(timer_spec));
    timer_spec.it_value.tv_sec = delay;
    timer_spec.it_value.tv_nsec = 0;
    timer_spec.it_interval.tv_sec = interval;
    timer_spec.it_interval.tv_nsec = 0;

    if (timerfd_settime(tfd, 0, &timer_spec, NULL) != 0) {
        printf("ERROR start_timer_listening::%d\n",errno);
        return ERR_TIMERFD_SETTIME;
    }

    return tfd;

}

int read_timer(int fd, char buf[8])
{
    int bytes_read = read(fd,buf,8);
    if( bytes_read <= 0)
    {
        printf("ERROR start_timer_listening %d (fd: %d, errno: %d)\n",bytes_read,fd,errno);
        return ERR_TIMERFD_READ;
    }

    return bytes_read;
}


int stop_timer_listening(int fd)
{
    int rc = close(fd);
    if(rc != 0)
    {
        printf("ERROR start_timer_listening::%d\n",errno);
        return ERR_TIMERFD_CLOSE;
    }

    return TIMER_OK;
}
