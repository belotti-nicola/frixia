#ifndef EPOLL_H
#define EPOLL_H

#include "fepoll_codes.h"
#include "sys/epoll.h"
#include "../../../fevent/frixia_event.h"

#define KERNEL_HINT 5

int create_epoll();
int wait_epoll_events(int             fd,
                      int             max_events,
                      frixia_event_t *fevents);
int add_fd(int epoll,
           int fd,
           struct epoll_event *ev);
int stop_fd(int epoll_fd,
            int closing_fd);

#endif