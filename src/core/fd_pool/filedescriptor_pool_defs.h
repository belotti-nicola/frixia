#ifndef FRIXIA_FD_FS_H
#define FRIXIA_FD_FS_H

#define MAXSTR 10

#include "../frixia_common.h"


enum FD_POOL_EXIT_CODES{
    ADD_OK=1,
    REMOVE_OK=2,
    TCP_PORT_NOT_FOUND=3,

    NO_AVAILABLE_SPACE_IN_DATASTRUCTURE = -1,
    FD_NOT_FOUND = -2,
    POSITION_OUTOFBOUND = -3

};

struct FrixiaFD
{
    int fd;                     // filedescriptor
    enum FrixiaFDType type;       // filedescriptor type
    char filename[MAXSTR + 1]; //
    int port;                   //
};

#endif