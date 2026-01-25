#ifndef FRIXIA_INODE_H
#define FRIXIA_INODE_H

#include <frixia/frixia_result.h>
#include <stdbool.h>

typedef enum FRIXIA_INODE_FLAG 
{
#define FINODE_FLAG_MACRO(name, value) name,
#include "internal/finode_flags.def"
#undef FINODE_FLAG_MACRO
} FRIXIA_INODE_FLAG;

typedef enum FINODE_CODE
{
    #define X(name,value,description) name,
    #include "internal/finode_codes.def"
    #undef X

    __FINODE_SENTINEL__

} FINODE_CODE;

typedef struct FRIXIA_INODE_RESULT
{
    FINODE_CODE code;
    int          errno_code;
    
} FRIXIA_INODE_RESULT;

typedef struct FRIXIA_INODE_FD_RESULT
{
    int                  fd;
    FRIXIA_INODE_RESULT res;

} FRIXIA_INODE_FD_RESULT;

FRIXIA_RESULT_KIND finode_code_is_ok(FINODE_CODE code);
const char*        frixia_inode_flag_to_string(FRIXIA_INODE_FLAG f);
const char*        finode_code_description_string(FINODE_CODE code);


#endif

