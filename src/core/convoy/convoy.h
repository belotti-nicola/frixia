#ifndef CONVOY_H
#define CONVOY_H

#include "../frixia_common.h"
#include <stdint.h>

#include "fd.h"

typedef struct convoy
{
    int maximum_size;
    int size;
    frixia_file_descriptor_t filedescriptors[MAXIMUM_FD_NUMBER];

} convoy_t;

convoy_t *convoy_create();
void convoy_destroy(convoy_t *convoy);

void convoy_add_tcp_filedescriptor(convoy_t *c, int fd, const char *ip, int port, int bytes,FRIXIA_SUPPORTED_PROTOCOL_T protocol);
void convoy_add_udp_filedescriptor(convoy_t *c, int fd, const char *ip, int port, int bytes);
void convoy_add_fifo_filedescriptor(convoy_t *c, int fd, const char *path, int bytes);
void convoy_add_timer_filedescriptor(convoy_t *c, int fd, const char *id, int delay, int interval);
void convoy_add_scheduler_filedescriptor(convoy_t *c, int fd, int tick);
void convoy_add_scheduled_timer_filedescriptor(convoy_t *c, int fd);
void convoy_add_add_inode_filedescriptor(convoy_t *c, int fd, char *filepath);

// void convoy_register_http_method_callback(convoy_t *c,const char *ip, int port, const char *method,const char *path,void *(*fun)(void *), void *arg);
// void convoy_register_fins_command_callback(convoy_t *c, enum FrixiaFDType type, const char *ip, int port, uint8_t first, uint8_t second,void *(*fun)(void *), void *arg);
// void convoy_register_noprotocol_callback(convoy_t *c, enum FrixiaFDType type, const char *ip, int port, void *(*fun)(void *), void *arg);
// void convoy_register_noprotocol_tcp_callback(convoy_t *convoy,const char *ip,int port, void *(*fun)(void *), void *arg);

//void convoy_register_timer_callback(convoy_t *c, const char *id, void *fun, void *arg);
void convoy_copy_fd(convoy_t *c, int source_fd, int destination_fd);


#endif