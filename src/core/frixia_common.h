#ifndef FRIXIA_COMMON_H
#define FRIXIA_COMMON_H

// FD Data Structure size
#define MAXIMUM_FILEDESCRIPTORS 10


enum FrixiaFDType
{
    STD_INPUT,
    STD_OUTPUT,
    STD_ERR,
    TCP,
    UDP,
    FIFO,

    UNDEFINED
};

enum FrixiaFDType get_frixiafdtype_by_string(char s[]);

#endif