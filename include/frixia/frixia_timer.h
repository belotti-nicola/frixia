#ifndef FRIXIA_TIMER_H
#define FRIXIA_TIMER_H

#include <frixia/frixia_result.h>
#include <stdbool.h>

typedef enum 
{
    #define X(name,a,b) name,
    #include "internal/ftimer_codes.def"
    #undef X
    __FTIMER_SENTINEL__

} FTIMER_CODE;

typedef struct FRIXIA_TIMER_RESULT
{
    FTIMER_CODE code;
    int        errno_code;

} FRIXIA_TIMER_RESULT;

typedef struct FRIXIA_TIMER_FD_RESULT
{
    int                 fd;
    FRIXIA_TIMER_RESULT res;

} FRIXIA_TIMER_FD_RESULT;

FRIXIA_RESULT_KIND ftimer_code_is_ok(FTIMER_CODE code);
const char*        ftimer_code_to_string(FTIMER_CODE code);
const char*        ftimer_code_string_description(FTIMER_CODE code);

#endif