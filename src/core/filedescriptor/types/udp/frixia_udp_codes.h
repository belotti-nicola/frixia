#ifndef FRIXIA_UDP_CODES_H
#define FRIXIA_UDP_CODES_H

#define FUDP_CODES_LIST \
    X(FUDP_OK) \
    X(ERR_FUDP_START_SOCKET) \
    X(ERR_FUDP_START_BIND) \
    X(ERR_FUDP_START_LISTEN) \
    X(ERR_FUDP_START_EPOLL_ADD) \
    X(ERR_FUDP_STOP_EPOLLCTL_DEL) \
    X(ERR_FUDP_ACCEPTING) \
    X(ERR_FUDP_READING) \
    X(ERR_FUDP_START_MALFORMED_EPOLL_FD) \
    X(ERR_FUDP_START_MALFORMED_PORT) \ 
    X(ERR_FUDP_STOP_MALFORMED_EPOLL_FD) \
    X(ERR_FUDP_STOP_MALFORMED_TARGET_FD) \ 
    X(ERR_FUDP_WRITING) \
    X(ERR_FUDP_CREATING_SOCKET)

typedef enum {
#define X(name) name,
    FUDP_CODES_LIST
#undef X
} FUDP_CODE_T ;


static inline const char* fudp_code_to_string(FUDP_CODE_T code) {
    switch(code) {
#define X(name) case name: return #name;
        FUDP_CODES_LIST
#undef X
    }
    return "UNKNOWN_UDP_CODE";
}


#endif