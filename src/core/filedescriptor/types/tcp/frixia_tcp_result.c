#include "frixia_tcp_result.h"

FRIXIA_TCP_RESULT ftcp_create_tcp_result(FTCP_CODE_T code, int errno_code)
{
    FRIXIA_TCP_RESULT retVal = { .exit_code = code, .errno_code = errno_code };
    return retVal;
}
