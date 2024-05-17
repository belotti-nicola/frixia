#include "frixia_http_request.h"
#include "../core/fd_pool/filedescriptor_pool.h"

int start_tcp_listening(struct FrixiaFD f_fd[],
                        int f_fd_size,
                        int epoll_fd,
                        int port);
int stop_tcp_listening(int closing_fd,
                       struct FrixiaFD f_fd[],
                       int max_size, int epoll_fd);

int read_tcp_socket(int fd);