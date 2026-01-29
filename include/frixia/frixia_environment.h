#ifndef FRIXIA_ENVIRONMENT_H
#define FRIXIA_ENVIRONMENT_H

#include "frixia_inode.h"
#include "frixia_signal.h"

#include <internal/convoy.h>
#include <frixia/frixia_shinsu_senju_th.h>
#include <frixia/frixia_dispatcher_th.h>
#include <frixia/frixia_tcp.h>
#include <frixia/frixia_result.h>
#include <sys/signalfd.h>

typedef struct fepoll_th_data fepoll_th_data_t;
typedef struct frixia_events_queue frixia_events_queue_t;
typedef struct frixia_environment
{
    int maximum_filedescriptors;
    int filedescriptors; 

    frixia_events_queue_t *fepoll_events;
    //dispatcher_events

    fepoll_th_data_t *fepoll_ctx;
    frixia_dispatcher_data_t *fdispatcher_ctx;
    shinsu_senju_data_t *shinsu_senju_ctx;

    convoy_t *convoy;

    sigset_t threads_sigset;

} frixia_environment_t;

frixia_environment_t *frixia_environment_create();
void                  frixia_environment_destroy(frixia_environment_t *fenv);
int                   frixia_start(frixia_environment_t *env);
int                   frixia_stop(frixia_environment_t *env);

FRIXIA_RESULT frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
FRIXIA_RESULT frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
FRIXIA_RESULT frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read);
FRIXIA_RESULT frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval);
FRIXIA_RESULT frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG mask);
FRIXIA_RESULT frixia_add_signal(frixia_environment_t *env, FRIXIA_SIGNAL mask);
FRIXIA_RESULT frixia_add_eventfd(frixia_environment_t *env);

void frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg);


#endif