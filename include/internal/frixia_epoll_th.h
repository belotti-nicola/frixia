#ifndef FRIXIA_EPOLL_H
#define FRIXIA_EPOLL_H

#include <pthread.h>
#include <stdbool.h>

#include <frixia/frixia_environment.h>
#include <internal/simple_valid_callback.h>

typedef struct frixia_epoll frixia_epoll_t;

typedef struct fepoll_th_data
{
    frixia_epoll_t *fepoll;
    pthread_t th;
    bool *keep_looping;
    sv_callback_t *callbacks;

    frixia_environment_t *fenv;

} fepoll_th_data_t;



fepoll_th_data_t *fepoll_th_data_create();
void *fepoll_th_data_destroy(fepoll_th_data_t *p);

int detached_start_epoll(fepoll_th_data_t *fepoll);
int detached_stop_epoll(fepoll_th_data_t *fepoll);
int detached_join_epoll(fepoll_th_data_t *fepoll);


#endif