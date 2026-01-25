#ifndef FRIXIA_UDP_H
#define FRIXIA_UDP_H

#include <stdbool.h>

typedef enum 
{
#define X(name,a,b) name,
#include "internal/fudp_codes.def"
#undef X
} FUDP_CODE;

bool        fudp_code_is_ok(FUDP_CODE code);
const char* fudp_code_to_string(FUDP_CODE code);

typedef struct FRIXIA_UDP_RESULT
{
    FUDP_CODE exit_code;
    int errno_code;
    
} FRIXIA_UDP_RESULT;

typedef struct FRIXIA_UDP_FD_RESULT
{
    int fd;
    FRIXIA_UDP_RESULT res;
    
} FRIXIA_UDP_FD_RESULT;


#endif