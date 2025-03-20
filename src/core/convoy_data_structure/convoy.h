#include "../frixia_common.h"

#include "fd.h"

typedef struct convoy 
{
    int                      size;
    frixia_file_descriptor_t filedescriptors[MAXIMUM_FD_NUMBER];

} convoy_t;


void convoy_add_tcp_filedescriptor(convoy_t *c,int fd,const char *ip,int port,int bytes);
void convoy_add_udp_filedescriptor(convoy_t *c,int fd,const char *ip,int port,int bytes);
void convoy_add_fifo_filedescriptor(convoy_t *c,int fd,const char *path,int bytes);
void convoy_add_timer_filedescriptor(convoy_t *c,int fd,const char *id, int delay, int interval);
void convoy_add_scheduler_filedescriptor(convoy_t *c, int fd, int tick);
void convoy_register_http_callback(convoy_t *c,const char *method,const char *path,void *fun, void *arg);
void convoy_register_fins_callback(convoy_t *c);
void convoy_register_timer_callback(convoy_t *c,const char *id,void *fun,void *arg);                                             
