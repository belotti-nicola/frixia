#ifndef FRIXIA_TIMER_H
#define FRIXIA_TIMER_H

typedef enum 
{
#define X(name) name,
#include "internal/ftimer_codes.def"
#undef X
} TIMER_CODE;

#endif