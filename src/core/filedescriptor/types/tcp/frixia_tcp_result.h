#ifndef FRIXIA_TCP_RESULT_H
#define FRIXIA_TCP_RESULT_H

#include "frixia_tcp_codes.h"

typedef struct FRIXIA_TCP_RESULT
{
    int fd;
    FTCP_CODE_T exit_code;
    int errno_code;
    
} FRIXIA_TCP_RESULT;


FRIXIA_TCP_RESULT ftcp_create_result(int fd, FTCP_CODE_T code, int errno_code)
{
    return (FRIXIA_TCP_RESULT){ .fd = fd, .exit_code = code, errno_code = errno_code  };
}

void ftcp_print_result(FRIXIA_TCP_RESULT r)
{
    printf("%s:%d %s %d(%s)\n",__FUNCTION__,r.fd,X(r.exit_code),"strerror(r.errno_code)");
}

#endif