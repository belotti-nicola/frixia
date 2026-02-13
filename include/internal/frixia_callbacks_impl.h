#ifndef FRIXIA_CALLBACKS_IMPL_H
#define FRIXIA_CALLBACKS_IMPL_H

typedef struct frixia_environment frixia_environment_t;
typedef struct FRIXIA_CALLBACK_CTX FRIXIA_CALLBACK_CTX;
typedef struct FRIXIA_EPOLL_CALLBACK_CTX FRIXIA_EPOLL_CALLBACK_CTX;
typedef struct FRIXIA_DISPATCHER_CALLBACK_CTX FRIXIA_DISPATCHER_CALLBACK_CTX;
typedef struct FRIXIA_SHINSU_SENJU_CALLBACK_CTX FRIXIA_SHINSU_SENJU_CALLBACK_CTX;

void frixia_register_cb_impl(frixia_environment_t *env,int fd, void *(fun)(FRIXIA_CALLBACK_CTX *), void *arg);
void frixia_register_fepoll_cb_impl(frixia_environment_t *env,int fd,  void *(fun)(FRIXIA_EPOLL_CALLBACK_CTX *), void *arg);
void frixia_register_dispatcher_cb_impl(frixia_environment_t *env,int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *),void * arg);
void frixia_register_shinsu_senju_cb_impl(frixia_environment_t *env, int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *),void * arg);


#endif