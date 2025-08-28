#ifndef FRIXIA_ENVIRONMENT_H
#define FRIXIA_ENVIRONMENT_H

#include "../fdispatcher/detached_frixia_dispatcher_new.h"
#include "../filedescriptor/fd_monitor/epoll/fepoll.h"
#include "../filedescriptor/fd_monitor/detached_epoll_monitor.h"
#include "../fdispatcher/frixia_dispatcher.h"

typedef struct frixia_environment
{
    int maximum_filedescriptors;
    int filedescriptors;

    frixia_dispatcher_t *fdispatcher;
    //thread_pool
    

    frixia_events_queue_t *fepoll_events;
    //dispatcher_events

    fepoll_th_data_t *fepoll_ctx;
    frixia_dispatcher_data_t *fdispatcher_ctx;

} frixia_environment_t;

frixia_environment_t *fenv_create(int maximum_filedescriptors);
void fenv_destroy(frixia_environment_t *fenv);


void fenv_start_tcp_listening(frixia_environment_t *env,const char *ip, int port);
void fenv_start_udp_listening(frixia_environment_t *env,const char *ip, int port);
void fenv_start_fifo_listening(frixia_environment_t *env, const char *pipe);
void *fenv_set_custom_tcp_callback(frixia_environment_t *env,const char *ip, int port, void *(*fun)(void *), void *arg);
void *fenv_set_custom_udp_callback(frixia_environment_t *env,const char *ip, int port, void *(*fun)(void *), void *arg);
void *fenv_set_custom_fifo_callback(frixia_environment_t *env,const char *path, void *(*fun)(void *), void *arg);


int fenv_run_engine(frixia_environment_t *fenv);
void wait_all_components(frixia_environment_t *fenv);

#endif