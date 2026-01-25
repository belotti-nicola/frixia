#ifndef FRIXIA_SIGNAL_H
#define FRIXIA_SIGNAL_H

#include <stdbool.h>

typedef enum 
{
#define FRIXIA_SIGNAL_MACRO(name,value) name,
#include "internal/frixia_signals.def"
#undef FRIXIA_SIGNAL_MACRO
} FRIXIA_SIGNAL;

typedef enum 
{
#define X(name,value,desc) name,
#include "internal/fsignal_codes.def"
#undef X
} FSIGNAL_CODE;


typedef struct FRIXIA_SIGNAL_ADD_RESULT
{
    int fd;
    FSIGNAL_CODE code;
    int errno_code;
    
} FRIXIA_SIGNAL_ADD_RESULT;

#include <stdbool.h>
bool        fsignal_code_is_ok(FSIGNAL_CODE code);
int frixia_signal_to_unix(FRIXIA_SIGNAL fsignal);


#endif