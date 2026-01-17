#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include <frixia/frixia_fifo.h>

int start_fifo_listening(const char *name)
{
    printf("start_fifo_listening '%s'\n", name);
    if (mkfifo(name, 0666) == -1)
    {
        return ERR_FFIFO_MKFIFO;
    }
    int fd = open(name, O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        return ERR_FFIFO_OPEN;
    }
    return fd;
}

int stop_fifo_listening(int closing_fd)
{
    close(closing_fd);
    return 0;
}

int read_fifo(int fd,
              char buf[],
              int size)
{
    int bytes_read = read(fd, buf, size);
    if (bytes_read == -1)
    {
        return ERR_FFIFO_READ;
    }
    return bytes_read;
}