#ifndef FRIXIA_TIMER_H
#define FRIXIA_TIMER_H

typedef enum 
{
#define X(name,a,b) name,
#include "internal/ftimer_codes.def"
#undef X
} TIMER_CODE;

typedef struct FRIXIA_TIMER_FD_RESULT
{
    int        fd;
    TIMER_CODE code;
    int        errno_code;

} FRIXIA_TIMER_FD_RESULT;


const char *ftimer_code_to_string(TIMER_CODE code);

#endif