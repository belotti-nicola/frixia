#ifndef FRIXIA_FIFO_H
#define FRIXIA_FIFO_H

#include <frixia/frixia_result.h>
#include <stdbool.h>

typedef enum 
{
    #define X(name,a,b) name,
    #include "internal/ffifo_codes.def"
    #undef X
    __FFIFO_SENTINEL__

} FFIFO_CODE;

typedef struct FRIXIA_FIFO_RESULT
{
    FFIFO_CODE code;
    int       errno_code;
    
} FRIXIA_FIFO_RESULT;

typedef struct FRIXIA_FIFO_FD_RESULT
{
    int               fd;
    FRIXIA_FIFO_RESULT res;

} FRIXIA_FIFO_FD_RESULT;

FRIXIA_RESULT_KIND ffifo_code_is_ok(FFIFO_CODE code);
const char*        ffifo_code_to_string(FFIFO_CODE code);
const char*        ffifo_code_description_string(FFIFO_CODE code);


#endif