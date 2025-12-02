#include <string.h>
#include <stdio.h>
#include "frixia_udp_result.h"

#include "frixia_udp_fd_result.h"

FRIXIA_UDP_FD_RESULT fudp_create_fd_result(int fd, FUDP_CODE_T code, int errno_code)
{
    FRIXIA_UDP_RESULT fres = fudp_create_udp_result(code,errno_code);
    FRIXIA_UDP_FD_RESULT ret = { .fd = fd, .res = fres  };
    return ret;
}

void fudp_print_fd_result(FRIXIA_UDP_FD_RESULT r)
{
    printf("%s:%d %s %d(%s)\n",
        __FUNCTION__,
        r.fd,
        fudp_code_to_string(r.res.exit_code),
        r.res.errno_code,
        strerror(r.res.errno_code)
    );
}