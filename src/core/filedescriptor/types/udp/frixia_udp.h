#ifndef FRIXIA_UDP_H
#define FRIXIA_UDP_H

enum FUDP_CODE {
    FUDP_OK,
    ERR_FUDP_START_SOCKET=-1,
    ERR_FUDP_START_BIND=-2,
    ERR_FUDP_START_LISTEN=-4,
    ERR_FUDP_START_EPOLL_ADD=-5,
    ERR_FUDP_STOP_EPOLLCTL_DEL=-6,
    ERR_FUDP_ACCEPTING=-7,
    ERR_FUDP_READING=-8,
    ERR_FUDP_START_MALFORMED_EPOLL_FD = -9,
    ERR_FUDP_START_MALFORMED_PORT = -10,
    ERR_FUDP_STOP_MALFORMED_EPOLL_FD = -11,
    ERR_FUDP_STOP_MALFORMED_TARGET_FD = -12
};
int start_udp_listening(int port);

int stop_udp_listening(int target_fd);

int read_udp(int fd, char buf[], int buf_size);

char* get_fudp_code_string(enum FUDP_CODE c);
int get_fudp_code_string_from_string(char *s);

#endif