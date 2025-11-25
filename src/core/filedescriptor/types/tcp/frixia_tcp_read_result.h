#ifndef FRIXIA_TCP_READ_RESULT_H
#define FRIXIA_TCP_READ_RESULT_H

#include "frixia_tcp_result.h"

typedef struct FRIXIA_TCP_READ_RESULT
{
    int bytes_read;
    FRIXIA_TCP_RESULT res;
    
} FRIXIA_TCP_READ_RESULT;

FRIXIA_TCP_READ_RESULT frixia_tcp_read_create(FTCP_CODE_T code, int size,int errno_code);

#endif