enum FUDP_CODE {
    FUDP_OK,
    ERR_FUDP_SOCKET=-1,
    ERR_FUDP_BIND=-2,
    ERR_FUDP_LISTEN=-4,
    ERR_FUDP_EPOLL_ADD=-5,
    ERR_FUDP_STOP=-6,
    ERR_FUDP_ACCEPTING=-7,
    ERR_FUDP_READING=-8
};
int start_udp_listening(int epoll_fd,
                        int port);

int stop_udp_listening(int epoll_fd,
                       int target_fd);