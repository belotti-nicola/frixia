#ifndef FRIXIA_FIFO_H
#define FRIXIA_FIFO_H

typedef enum 
{
#define X(name,a,b) name,
#include "internal/ffifo_codes.def"
#undef X
} FFIFO_CODE;

typedef struct FRIXIA_FIFO_FD_RESULT
{
    int        fd;
    FFIFO_CODE code;
    int        errno_code;

} FRIXIA_FIFO_FD_RESULT;

const char* ffifo_code_to_string(FFIFO_CODE code);

#endif