int start_fifo_listening(int epoll_fd,
                         char *name);

int stop_fifo_listening(int epoll_fd,
                        int closing_fd);