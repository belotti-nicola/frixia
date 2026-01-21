#ifndef FRIXIA_SIGNAL_H
#define FRIXIA_SIGNAL_H

typedef enum 
{
#define X(name,value,b) name,
#include "internal/fsignal_codes.def"
#undef X
} FRIXIA_SIGNAL;

int frixia_signal_to_unix(FRIXIA_SIGNAL fsignal);


#endif