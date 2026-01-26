#ifndef FRIXIA_TCP_H
#define FRIXIA_TCP_H

#include <frixia/frixia_result.h>
#include <stdbool.h>

typedef enum 
{
    #define X(name,value,desc) name,
    #include "internal/ftcp_codes.def"
    #undef X
    __FTCP_SENTINEL__

} FTCP_CODE;

FRIXIA_RESULT_KIND ftcp_code_is_ok(FTCP_CODE code);
const char*        ftcp_code_to_string(FTCP_CODE code);
const char*        ftcp_code_description_string(FTCP_CODE code);

typedef struct FRIXIA_TCP_RESULT
{
    FTCP_CODE exit_code;
    int errno_code;
    
} FRIXIA_TCP_RESULT;

typedef struct FRIXIA_TCP_FD_RESULT
{
    int fd;
    FRIXIA_TCP_RESULT res;
    
} FRIXIA_TCP_FD_RESULT;

typedef struct FRIXIA_TCP_READ_RESULT
{
    int bytes_read;
    FRIXIA_TCP_RESULT res;
    
} FRIXIA_TCP_READ_RESULT;

typedef struct FRIXIA_TCP_WRITE_RESULT
{
    int bytes_written;
    FRIXIA_TCP_RESULT res;
    
} FRIXIA_TCP_WRITE_RESULT;

#endif