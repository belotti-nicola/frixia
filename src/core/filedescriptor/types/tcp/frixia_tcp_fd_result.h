#ifndef FRIXIA_TCP_FD_RESULT_H
#define FRIXIA_TCP_FD_RESULT_H

#include "frixia_tcp_codes.h"
#include "frixia_tcp_result.h"

typedef struct FRIXIA_TCP_FD_RESULT
{
    int fd;
    FRIXIA_TCP_RESULT res;
    
} FRIXIA_TCP_FD_RESULT;


FRIXIA_TCP_FD_RESULT ftcp_create_fd_result(int fd, FTCP_CODE_T code, int errno_code);
void ftcp_print_fd_result(FRIXIA_TCP_FD_RESULT r);

#endif