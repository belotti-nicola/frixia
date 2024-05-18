#include "../core/fd_pool/filedescriptor_pool.h"

int start_udp_listening(struct FrixiaFD f_fd[],
                        int max_size,
                        int epoll_fd,
                        int port);

int stop_udp_listening(int closing_fd,
                       struct FrixiaFD f_fd[],
                       int max_size,
                       int epoll_fd);