#ifndef FRIXIA_EVENTFD_H
#define FRIXIA_EVENTFD_H

#include <frixia/frixia_result.h>
#include <stdbool.h>

typedef enum FEVENTFD_CODE {
    #define X(name,value,description) name,
    #include "internal/feventfd_codes.def"
    #undef X
    __FEVENTFD_SENTINEL__

} FEVENTFD_CODE;

typedef struct FRIXIA_EVENTFD_RESULT
{
    FEVENTFD_CODE code;
    int           errno_code;
    
} FRIXIA_EVENTFD_RESULT;

typedef struct FRIXIA_EVENTFD_FD_RESULT
{
    int                   fd;
    FRIXIA_EVENTFD_RESULT res;
    
} FRIXIA_EVENTFD_FD_RESULT;

FRIXIA_RESULT_KIND feventfd_code_is_ok(FEVENTFD_CODE code);
const char*        frixia_eventfd_to_string(FEVENTFD_CODE f);
const char*        feventfd_code_description_string(FEVENTFD_CODE code);


#endif