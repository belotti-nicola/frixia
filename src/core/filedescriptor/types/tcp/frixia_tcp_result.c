#include <string.h>

#include "frixia_tcp_result.h"

FRIXIA_TCP_RESULT ftcp_create_result(int fd, FTCP_CODE_T code, int errno_code)
{
    FRIXIA_TCP_RESULT ret = { .fd = fd, .exit_code = code, errno_code = errno_code  };
    return ret;
}

void ftcp_print_result(FRIXIA_TCP_RESULT r)
{
    printf("%s:%d %s %d(%s)\n",
        __FUNCTION__,
        r.fd,
        ftcp_code_to_string(r.exit_code),
        r.errno_code,
        strerror(r.errno_code)
    );
}