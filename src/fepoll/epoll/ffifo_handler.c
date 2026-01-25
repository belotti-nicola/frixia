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

FRIXIA_FIFO_FD_RESULT CREATE_FIFO_FD_RESULT(int fd,FFIFO_CODE code,int errno_code)
{
    FRIXIA_FIFO_FD_RESULT retVal = 
    {
        .fd             = fd,
        .res.code       = code,
        .res.errno_code = errno_code,
    };
    return retVal;
}

FRIXIA_FIFO_FD_RESULT start_fifo_listening(const char *name)
{
    printf("start_fifo_listening '%s'\n", name);
    if (mkfifo(name, 0666) == -1)
    {
        return CREATE_FIFO_FD_RESULT(-1,ERR_FFIFO_MKFIFO,errno);
    }
    int fd = open(name, O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        return CREATE_FIFO_FD_RESULT(-1,ERR_FFIFO_OPEN,errno);
    }
    
    return CREATE_FIFO_FD_RESULT(fd,FFIFO_OK,-1);
}

FRIXIA_FIFO_FD_RESULT stop_fifo_listening(int closing_fd)
{
    close(closing_fd);
    return CREATE_FIFO_FD_RESULT(-1,FFIFO_OK,-1);
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