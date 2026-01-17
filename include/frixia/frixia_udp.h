#ifndef FRIXIA_UDP_H
#define FRIXIA_UDP_H


typedef enum 
{
#define X(name) name,
#include "internal/fudp_codes.def"
#undef X
} FUDP_CODE;


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