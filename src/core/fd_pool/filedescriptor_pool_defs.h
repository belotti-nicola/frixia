#ifndef FRIXIA_FD_FS_H
#define FRIXIA_FD_FS_H

#define MAXSTR 10

enum FrixiaFDType
{
    STD_INPUT,
    STD_OUTPUT,
    STD_ERR,
    TCP,
    UDP,
    PIPE,

    UNDEFINED
};

enum FD_POOL_EXIT_CODES{
    ADD_OK=1,
    REMOVE_OK=2,

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