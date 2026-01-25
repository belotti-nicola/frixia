#ifndef FRIXIA_INODE_H
#define FRIXIA_INODE_H

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

} FINODE_CODE;


typedef struct FRIXIA_INODE_ADD_RESULT
{
    int fd;
    FINODE_CODE code;
    int errno_code;
    
} FRIXIA_INODE_ADD_RESULT;

bool        finode_code_is_ok(FINODE_CODE code);
const char *frixia_inode_flag_to_string(FRIXIA_INODE_FLAG f);


#endif

