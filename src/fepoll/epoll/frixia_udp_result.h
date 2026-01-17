#ifndef FRIXIA_UDP_RESULT_H
#define FRIXIA_UDP_RESULT_H

#include "frixia_udp_codes.h"

typedef struct FRIXIA_udp_RESULT
{
    FUDP_CODE_T exit_code;
    int errno_code;
    
} FRIXIA_UDP_RESULT;


FRIXIA_UDP_RESULT fudp_create_udp_result(FUDP_CODE_T code, int errno_code);

#endif