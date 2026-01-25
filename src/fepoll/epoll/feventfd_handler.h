#ifndef FEVENTFD_HANDLER_H
#define FEVENTFD_HANDLER_H

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <errno.h>

#include <frixia/frixia_eventfd.h>

FRIXIA_EVENTFD_FD_RESULT start_eventfd_listening();
int read_eventfd(int fd);
FRIXIA_EVENTFD_FD_RESULT close_eventfd(int fd);
int write_eventfd(int fd);

#endif