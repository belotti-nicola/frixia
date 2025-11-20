#ifndef FRIXIA_TCP_CODES_H
#define FRIXIA_TCP_CODES_H




#define FTCP_CODES_LIST \
    X(FTCP_OK) \
    X(ERR_FTCP_SOCKET) \
    X(ERR_FTCP_SETSOCKETOPT) \
    X(ERR_FTCP_BIND) \
    X(ERR_FTCP_LISTEN) \
    X(ERR_FTCP_START_EPOLLCTL_ADD) \
    X(ERR_FTCP_STOP_EPOLLCTL_DEL) \
    X(ERR_FTCP_ACCEPTING) \
    X(ERR_FTCP_READING) \
    X(ERR_FTCP_START_MALFORMED_EPOLL_FD) \
    X(ERR_FTCP_START_MALFORMED_PORT) \
    X(ERR_FTCP_STOP_MALFORMED_EPOLL_FD) \
    X(ERR_FTCP_STOP_MALFORMED_TARGET_FD) \
    X(ERR_FTCP_WRITE) \
    X(ERR_FTCP_FNCTL) \
    X(ERR_FTCP_INET_PTON) \
    X(ERR_FTCP_IP_NOT_VALID) 

typedef enum {
#define X(name) name,
    FTCP_CODES_LIST
#undef X
} FTCP_CODE_T ;


const char* ftcp_code_to_string(FTCP_CODE_T code) {
    switch(code) {
#define X(name) case name: return #name;
        FTCP_CODES_LIST
#undef X
    }
    return "UNKNOWN_FTCP_CODE";


#endif