int start_fifo_listening(int epoll_fd,
                         const char *name);

int stop_fifo_listening(int epoll_fd,
                        int closing_fd);
int read_fifo_fd(int fd,
                 char *buf,
                 int size);