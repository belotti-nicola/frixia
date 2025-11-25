#include <string.h>
#include <stdio.h>
#include "frixia_tcp_result.h"

#include "frixia_tcp_fd_result.h"

FRIXIA_TCP_FD_RESULT ftcp_create_fd_result(int fd, FTCP_CODE_T code, int errno_code)
{
    FRIXIA_TCP_RESULT fres = ftcp_create_tcp_result(code,errno_code);
    FRIXIA_TCP_FD_RESULT ret = { .fd = fd, .res = fres  };
    return ret;
}

void ftcp_print_fd_result(FRIXIA_TCP_FD_RESULT r)
{
    printf("%s:%d %s %d(%s)\n",
        __FUNCTION__,
        r.fd,
        ftcp_code_to_string(r.res.exit_code),
        r.res.errno_code,
        strerror(r.res.errno_code)
    );
}