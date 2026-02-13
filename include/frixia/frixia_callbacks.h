#ifndef FRIXIA_CALLBACK_H
#define FRIXIA_CALLBACK_H


//FORWARD DECLARATION
#include <frixia/frixia_environment.h>
#include <internal/frixia_callbacks_impl.h>

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

#define frixia_register_cb(env, fd, fun, arg) \
    do { \
        _Static_assert( \
            __builtin_types_compatible_p(__typeof__(fun), void *(*)(FRIXIA_CALLBACK_CTX *)), \
            "FRIXIA: tipo callback sbagliato" \
        ); \
        frixia_register_cb_impl(env, fd, fun, arg); \
    } while(0)

#define frixia_register_fepoll_cb(env, fd, fun, arg) \
    do { \
        _Static_assert( \
            __builtin_types_compatible_p(__typeof__(fun), void *(*)(FRIXIA_FEPOLL_CALLBACK_CTX *)), \
            "FRIXIA: tipo callback sbagliato" \
        ); \
        frixia_register_fepoll_cb_impl(env, fd, fun, arg); \
    } while(0)

#define frixia_register_dispatcher_cb(env, fd, fun, arg) \
    do { \
        _Static_assert( \
            __builtin_types_compatible_p(__typeof__(fun), void *(*)(FRIXIA_DISPATCHER_CALLBACK_CTX *)), \
            "FRIXIA: tipo callback sbagliato" \
        ); \
        frixia_register_dispatcher_cb_impl(env, fd, fun, arg); \
    } while(0)

#define frixia_register_shinsu_senju_cb(env, fd, fun, arg) \
    do { \
        _Static_assert( \
            __builtin_types_compatible_p(__typeof__(fun), void *(*)(FRIXIA_SHINSU_SENJU_CALLBACK_CTX *)), \
            "FRIXIA: tipo callback sbagliato" \
        ); \
        frixia_register_shinsu_senju_cb_impl(env, fd, fun, arg); \
    } while(0)


#endif