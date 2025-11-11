#ifndef FRIXIA_EVENTFD_H
#define FRIXIA_EVENTFD_H

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>


#include "frixia_eventfd.h"

int start_eventfd_listening();
int read_eventfd(int fd);
int close_eventfd(int fd);
int write_eventfd(int fd);

#endif