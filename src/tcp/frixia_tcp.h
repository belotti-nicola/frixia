enum FTCP_CODE {
    FTCP_OK,
    ERR_FTCP_SOCKET=-1,
    ERR_FTCP_SETSOCKETOPT=-2,
    ERR_FTCP_BIND=-3,
    ERR_FTCP_LISTEN=-4,
    ERR_FTCP_START_EPOLLCTL_ADD=-5,
    ERR_FTCP_STOP_EPOLLCTL_DEL=-6,
    ERR_FTCP_ACCEPTING=-7,
    ERR_FTCP_READING=-8,
    ERR_FTCP_START_MALFORMED_EPOLL_FD = -9,
    ERR_FTCP_START_MALFORMED_PORT = -10,
    ERR_FTCP_STOP_MALFORMED_EPOLL_FD = -11,
    ERR_FTCP_STOP_MALFORMED_TARGET_FD = -12
};

int start_tcp_listening(int epoll_fd,
                        int port);
int stop_tcp_listening(int epoll_fd,
                       int fd);
int read_tcp_socket(int fd,char* buf[], int size);

char* get_ftcp_code_string(enum FTCP_CODE);
int get_ftcp_code_string_from_string(char *s);