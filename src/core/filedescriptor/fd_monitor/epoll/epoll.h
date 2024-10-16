#include "fepoll_codes.h"

#define KERNEL_HINT 5

int start_epoll();
int stop_epoll();
int add_tcp_listener();
int stop_tcp_listener();
