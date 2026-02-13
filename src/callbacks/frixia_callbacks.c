#include <frixia/frixia_callbacks.h>
#include <frixia/frixia_environment.h>

void frixia_register_cb_impl(frixia_environment_t *env,int fd, void *(fun)(FRIXIA_CALLBACK_CTX *ctx), void *arg)
{/*todo*/}
void frixia_register_fepoll_cb_impl(frixia_environment_t *env,int fd,  void *(fun)(FRIXIA_EPOLL_CALLBACK_CTX *ctx), void *arg)
{/*todo*/}
void frixia_register_dispatcher_cb_impl(frixia_environment_t *env,int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}
void frixia_register_shinsu_senju_cb_impl(frixia_environment_t *env, int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}