#include "frixia_tcp_result.h"


typedef struct FRIXIA_TCP_WRITE_RESULT
{
    int bytes_written;
    FRIXIA_TCP_RESULT res;
    
} FRIXIA_TCP_WRITE_RESULT;

FRIXIA_TCP_WRITE_RESULT frixia_tcp_write_create(FTCP_CODE_T code, int size);
