#ifndef FRIXIA_TIMER_H
#define FRIXIA_TIMER_H

#include <stdbool.h>

typedef enum 
{
#define X(name,a,b) name,
#include "internal/ftimer_codes.def"
#undef X
} FTIMER_CODE;

typedef struct FRIXIA_TIMER_FD_RESULT
{
    int        fd;
    FTIMER_CODE code;
    int        errno_code;

} FRIXIA_TIMER_FD_RESULT;

bool        ftimer_code_is_ok(FTIMER_CODE code);
const char *ftimer_code_to_string(FTIMER_CODE code);

#endif