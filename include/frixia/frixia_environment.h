#ifndef FRIXIA_ENVIRONMENT_H
#define FRIXIA_ENVIRONMENT_H

#include "frixia_inode.h"
#include "frixia_signal.h"

typedef struct frixia_environment frixia_environment_t;

frixia_environment_t *frixia_environment_create();
void                  frixia_environment_destroy(frixia_environment_t *fenv);
int                   frixia_start(frixia_environment_t *env);
int                   frixia_stop(frixia_environment_t *env);

void frixia_add_tcp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
void frixia_add_udp(frixia_environment_t *env,char *ip,int port,int bytes_to_read);
void frixia_add_fifo(frixia_environment_t *env,const char *file, int bytes_to_read);
void frixia_add_timer(frixia_environment_t *env,const char *id, int delay, int interval);
void frixia_add_inode(frixia_environment_t *env, char *filepath, FRIXIA_INODE_FLAG mask);
void frixia_add_signal(frixia_environment_t *env, char *filepath, FRIXIA_SIGNAL mask);
void frixia_add_eventfd(frixia_environment_t *env);

void frixia_register_callback(frixia_environment_t *env, int fd,void *(fun)(void *),void *arg);





#endif