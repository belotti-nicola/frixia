#ifndef FRIXIA_SIGNAL_H
#define FRIXIA_SIGNAL_H

#include <frixia/frixia_result.h>
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
    __FSIGNAL_SENTINEL__
} FSIGNAL_CODE;


typedef struct FRIXIA_SIGNAL_RESULT
{
    FSIGNAL_CODE code;
    int          errno_code;
    
} FRIXIA_SIGNAL_RESULT;

typedef struct FRIXIA_SIGNAL_FD_RESULT
{
    int                  fd;
    FRIXIA_SIGNAL_RESULT res;

} FRIXIA_SIGNAL_FD_RESULT;

FRIXIA_RESULT_KIND fsignal_code_is_ok(FSIGNAL_CODE code);
const char*        fsignal_code_string_description(FSIGNAL_CODE code);
int                frixia_signal_to_unix(FRIXIA_SIGNAL fsignal);


#endif