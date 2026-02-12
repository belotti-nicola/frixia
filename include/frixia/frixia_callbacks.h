#ifndef FRIXIA_CALLBACK_H
#define FRIXIA_CALLBACK_H

#include <frixia/frixia_environment.h>

typedef struct FRIXIA_CALLBACK_CTX
{
    int fd;
    bool *keep_looping;
    frixia_environment_t *fenv;
   
} FRIXIA_CALLBACK_CTX;

typedef struct FRIXIA_EPOLL_CALLBACK_CTX
{
    //TODO
    //EPOLL CALLBACK TO REGISTER IN MAIN API
}
FRIXIA_EPOLL_CALLBACK_CTX;

typedef struct FRIXIA_DISPATCHER_CALLBACK_CTX
{
    //TODO
    //DISATCHER CALLBACK TO REGISTER IN MAIN API
}
FRIXIA_DISPATCHER_CALLBACK_CTX;

typedef struct FRIXIA_SHINSU_SENJU_CALLBACK_CTX
{
    //TODO
    //SHINSU_SENJU CALLBACK TO REGISTER IN MAIN API
}
FRIXIA_SHINSU_SENJU_CALLBACK_CTX;


void frixia_register_cb(int fd, FRIXIA_CALLBACK_CTX cb);
void frixia_register_fepoll_cb(int fd, FRIXIA_EPOLL_CALLBACK_CTX cb);
void frixia_register_dispatcher_cb(int fd, FRIXIA_DISPATCHER_CALLBACK_CTX cb);
void frixia_register_shinsu_senju_cb(int fd, FRIXIA_SHINSU_SENJU_CALLBACK_CTX cb);

#endif