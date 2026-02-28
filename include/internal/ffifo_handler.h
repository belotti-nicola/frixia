#ifndef FFIFO_HANDLER_H
#define FFIFO_HANDLER_H

#include <frixia/frixia_fifo.h>

FRIXIA_FIFO_FD_RESULT start_fifo_listening(const char *name);

FRIXIA_FIFO_FD_RESULT stop_fifo_listening(int closing_fd);
int read_fifo(int fd,
              char buf[],
              int size);

#endif