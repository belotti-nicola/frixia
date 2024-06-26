#ifndef FRIXIA_COMMON_H
#define FRIXIA_COMMON_H

// FD Data Structure size
#define MAXIMUM_FILEDESCRIPTORS 10

// FRIXIA COMMAND 
#define MAXIMUM_FRIXIA_ENGINE_COMMAND_LENGTH 40

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