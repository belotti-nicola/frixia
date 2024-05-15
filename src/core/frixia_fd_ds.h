#ifndef FRIXIA_FD_FS_H
#define FRIXIA_FD_FS_H

enum FrixiaType {
    STD_INPUT,
    STD_OUTPUT,
    STD_ERR,
    HTTP,
    UDP,
    PIPE,



    UNDEFINED
};

struct FrixiaFDDataStructure {
    int fd;         // filedescriptor
    enum FrixiaType type;// filedescriptor type 
};



#endif
