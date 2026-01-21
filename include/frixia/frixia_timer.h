#ifndef FRIXIA_TIMER_H
#define FRIXIA_TIMER_H

typedef enum 
{
#define X(name,a,b) name,
#include "internal/ftimer_codes.def"
#undef X
} TIMER_CODE;

const char *ftimer_code_to_string(TIMER_CODE code);

#endif