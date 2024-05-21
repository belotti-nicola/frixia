#include "frixia_http_request.h"
#include "../core/fd_pool/filedescriptor_pool.h"

int start_tcp_listening(int epoll_fd,
                        int port);
int stop_tcp_listening(int epoll_fd,
                       int fd);
int read_tcp_socket(int fd);