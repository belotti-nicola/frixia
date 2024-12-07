#ifndef FILEDESCRIPTOR_READER_H
#define FILEDESCRIPTOR_READER_H

#include "../../filedescriptor/fd_monitor/epoll/fepoll.h"

typedef void (*frixia_fd_reader)();

int read_frixia_filedescriptor(frixia_epoll_t *fepoll, int fd, char *s);

#endif