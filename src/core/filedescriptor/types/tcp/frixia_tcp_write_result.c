#include "frixia_tcp_result.h"

#include "frixia_tcp_write_result.h"

FRIXIA_TCP_WRITE_RESULT frixia_tcp_write_create(FTCP_CODE_T code, int bytes_written)
{
    FRIXIA_TCP_WRITE_RESULT res = {  .res = code, .bytes_written = bytes_written };
    return res; 
}