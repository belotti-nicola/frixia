#ifndef FRIXIA_INODE_H
#define FRIXIA_INODE_H

typedef enum FRIXIA_INODE_FLAG {
#define X(name, value,desc) name,
#include "internal/finode_codes.def"
#undef X
} FRIXIA_INODE_FLAG;

const char *frixia_inode_flag_to_string(FRIXIA_INODE_FLAG f);


#endif

