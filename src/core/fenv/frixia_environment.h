#ifndef FRIXIA_ENVIRONMENT_H
#define FRIXIA_ENVIRONMENT_H

#include "../filedescriptor/fd_monitor/epoll/fepoll.h"

typedef struct frixia_environment
{
    int maximum_filedescriptors;
    int filedescriptors;
    frixia_epoll_t *fepoll;
    //dispatcher 
    //thread_pool
    

    frixia_events_queue_t *fepoll_events;
    //dispatcher_events

} frixia_environment_t;

frixia_environment_t *fenv_create(int maximum_filedescriptors);
void fenv_destroy(frixia_environment_t *fenv);


void fenv_start_tcp_listening(frixia_environment_t *env,const char *ip, int port);
void fenv_start_udp_listening(frixia_environment_t *env,const char *ip, int port);
void fenv_start_fifo_listening(frixia_environment_t *env, const char *pipe);

void fenv_run_engine(frixia_environment_t *fenv);
void wait_all_components(frixia_environment_t *fenv);

#endif