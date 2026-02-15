#include <frixia/frixia_callbacks.h>
#include <frixia/frixia_environment.h>

void frixia_register_cb_impl(frixia_environment_t *env,int fd, void *(fun)(FRIXIA_CALLBACK_CTX *ctx), void *arg)
{
    void *(*f)(void *) = (void *(*)(void *))fun;

    shinsu_senju_data_t *ssd = env->shinsu_senju_ctx;
    detached_shinsu_senju_load(ssd,fd,f,arg);
}
void frixia_register_fepoll_cb_impl(frixia_environment_t *env,int fd,  void *(fun)(FRIXIA_EPOLL_CALLBACK_CTX *ctx), void *arg)
{/*todo*/}
void frixia_register_dispatcher_cb_impl(frixia_environment_t *env,int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}
void frixia_register_shinsu_senju_cb_impl(frixia_environment_t *env, int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}
void frixia_register_http_callback_impl(frixia_environment_t *env, int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}
void frixia_register_https_callback_impl(frixia_environment_t *env, int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}
void frixia_register_fins_callback_impl(frixia_environment_t *env, int fd,  void *(fun)(FRIXIA_DISPATCHER_CALLBACK_CTX *ctx),void * arg)
{/*todo*/}
