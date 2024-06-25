enum FFIFO_CODE
{
    OK,
    ERR_FFIFO_MKFIFO = -1,
    ERR_FFIFO_OPEN = -2,
    ERR_FFIFO_EPOLLCTL_ADD = -3,
    ERR_FFIFO_STOP = -4,
    ERR_FFIFO_READ = -5
};

int start_fifo_listening(int epoll_fd,
                         const char *name);

int stop_fifo_listening(int epoll_fd,
                        int closing_fd);
int read_fifo(int fd,
              char buf[],
              int size);