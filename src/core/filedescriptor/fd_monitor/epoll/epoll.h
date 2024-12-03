#ifndef EPOLL_H
#define EPOLL_H

#include "fepoll_codes.h"
#include "sys/epoll.h"
#include "../../../fevent/frixia_event.h"

#define KERNEL_HINT 5

int create_epoll();
int start_epoll(int epoll);
int stop_epoll(int fd);
int wait_epoll_events(int fd, int max,frixia_event_t *fevents);
int add_fd_listener(int epoll,
                    int fd,
                    struct epoll_event *ev);
int stop_fd_listener(int epoll_fd,
                     int closing_fd);

#endif