#ifndef FRIXIA_FD_FS_H
#define FRIXIA_FD_FS_H

#define MAXSTR 20

#include "../frixia_common.h"

enum FRIXIA_EVENT_DISPATCHER{
    NONE,       //DEFAULT
    PROGRAM,    //COMUNICATE WITH PROGRAM
    ENGINE      //MANIPULATE ENGINE TYPE
};

enum FD_POOL_EXIT_CODES{
    ADD_OK=1,
    REMOVE_OK=2,

    NO_AVAILABLE_SPACE_IN_DATASTRUCTURE = -1,
    FD_NOT_FOUND = -2,
    POSITION_OUTOFBOUND = -3,
    TCP_PORT_NOT_FOUND=-4,
    UDP_PORT_NOT_FOUND=-5,
    FIFO_NAME_NOT_FOUND=-6

};

struct FrixiaFD
{
    int fd;                                 // filedescriptor
    enum FRIXIA_EVENT_DISPATCHER dispatcher;// parse or read
    enum FrixiaFDType filedescriptor_type;  // filedescriptor type : IF UNDEFINED IS NOT USED
    char filename[MAXSTR + 1];              // fifos
    int port;                               // tcp/udp
};

#endif