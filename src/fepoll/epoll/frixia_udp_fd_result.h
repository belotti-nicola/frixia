#ifndef FRIXIA_UDP_FD_RESULT_H
#define FRIXIA_UDP_FD_RESULT_H

#include "frixia_udp_codes.h"
#include "frixia_udp_result.h"

typedef struct FRIXIA_UDP_FD_RESULT
{
    int fd;
    FRIXIA_UDP_RESULT res;
    
} FRIXIA_UDP_FD_RESULT;


FRIXIA_UDP_FD_RESULT fudp_create_fd_result(int fd, FUDP_CODE_T code, int errno_code);
void fudp_print_fd_result(FRIXIA_UDP_FD_RESULT r);

#endif