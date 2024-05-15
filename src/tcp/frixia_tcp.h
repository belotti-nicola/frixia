#include "frixia_http_request.h"

int start_tcp_listening(int fd, int epoll_fd, int port);
int stop_tcp_listening(int fd,int epoll_fd);

int read_tcp_socket(int fd);