int start_udp_listening(int epoll_fd,
                        int port);

int stop_udp_listening(int epoll_fd,
                       int target_fd);