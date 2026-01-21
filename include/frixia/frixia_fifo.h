#ifndef FRIXIA_FIFO_H
#define FRIXIA_FIFO_H

typedef enum 
{
#define X(name,a,b) name,
#include "internal/ffifo_codes.def"
#undef X
} FFIFO_CODE;

const char* ffifo_code_to_string(FFIFO_CODE code);

#endif