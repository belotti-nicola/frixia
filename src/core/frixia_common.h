#ifndef FRIXIA_COMMON_H
#define FRIXIA_COMMON_H

// FD Data Structure size
#define MAXIMUM_FILEDESCRIPTORS 10

// FRIXIA COMMAND 
#define MAXIMUM_FRIXIA_ENGINE_COMMAND_LENGTH 40

//STRING FOR PIPES
#define MAXSTR 20


#define FRIXIA_THREAD_POOL 5

// take first argument
// multiply by 2^8
// add b
#define KEY(a, b) ((a << 8 | b))

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
char* get_frixiafdtype(int t);
#endif