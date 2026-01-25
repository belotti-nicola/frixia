#ifndef FRIXIA_EVENTFD_H
#define FRIXIA_EVENTFD_H

#include <stdbool.h>

typedef enum FEVENTFD_CODE {
#define X(name,value,description) name,
#include "internal/feventfd_codes.def"
#undef X
} FEVENTFD_CODE;

typedef struct FRIXIA_ADD_FEVENTFD_RESULT
{
    int fd;
    FEVENTFD_CODE code;
    int errno_code;
    
} FRIXIA_ADD_FEVENTFD_RESULT;

bool        feventfd_code_is_ok(FEVENTFD_CODE code);
const char *frixia_eventfd_to_string(FEVENTFD_CODE f);


#endif