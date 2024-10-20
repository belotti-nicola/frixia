#ifndef EPOLL_H
#define EPOLL_H

#include "fepoll_codes.h"
#include "sys/epoll.h"

#define KERNEL_HINT 5

int start_epoll();
int stop_epoll(int fd);
int add_fd_listener(int epoll,
                    int fd,
                    struct epoll_event *ev);
int stop_fd_listener(int epoll_fd,
                     int closing_fd);

#endif