#ifndef FRIXIA_COMMON_H
#define FRIXIA_COMMON_H

// FRIXIA COMMAND 
#define MAXIMUM_FRIXIA_ENGINE_COMMAND_LENGTH 40

//STRING FOR PIPES
#define MAXSTR 20

//THREAD POOL SIZE
#define FRIXIA_WORKERS 5

// take first argument
// multiply by 2^8
// add b
#define KEY(a, b) ((a << 8 | b))

#define MAXIMUM_FD_NUMBER 10

//epoll_wait size
#define FRIXIA_EPOLL_MAXIMUM_EVENTS 10

enum FrixiaFDType
{
    UNDEFINED,

    STD_INPUT,
    STD_OUTPUT,
    STD_ERR,
    TCP,
    UDP,
    FIFO,
    TIMER,
    WAKE,
    SCHEDULER,
    EVENTFD,
    INODE

};

enum FrixiaFDType get_frixiafdtype_by_string(char s[]);
char* get_frixiafdtype(int t);
#endif