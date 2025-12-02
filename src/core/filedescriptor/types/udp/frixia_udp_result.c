#include "frixia_udp_result.h"

FRIXIA_UDP_RESULT fudp_create_udp_result(FUDP_CODE_T code, int errno_code)
{
    FRIXIA_UDP_RESULT retVal = { .exit_code = code, .errno_code = errno_code };
    return retVal;
}