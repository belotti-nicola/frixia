#include "frixia_tcp_result.h"

#include "frixia_tcp_read_result.h"

FRIXIA_TCP_READ_RESULT frixia_tcp_read_create(FTCP_CODE_T code, int bytes_read,int errno_code)
{
    FRIXIA_TCP_RESULT res = { .exit_code=code, .errno_code=errno_code };
    FRIXIA_TCP_READ_RESULT retVal = {  .res = res, .bytes_read = bytes_read };
    return retVal; 
}