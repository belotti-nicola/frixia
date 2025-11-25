#ifndef FRIXIA_TCP_RESULT_H
#define FRIXIA_TCP_RESULT_H

#include "frixia_tcp_codes.h"

typedef struct FRIXIA_TCP_RESULT
{
    FTCP_CODE_T exit_code;
    int errno_code;
    
} FRIXIA_TCP_RESULT;


FRIXIA_TCP_RESULT ftcp_create_tcp_result(FTCP_CODE_T code, int errno_code);

#endif