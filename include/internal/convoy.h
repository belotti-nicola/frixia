#ifndef CONVOY_H
#define CONVOY_H

#include <internal/frixia_fd_args.h>
#include <frixia/frixia_signal.h>
#include <stdint.h>
#include <pthread.h>

enum FrixiaFDType
{
    UNDEFINED,

    STD_INPUT,
    STD_OUTPUT,
    STD_ERR,
    TCP,
    UDP,
    FIFO,
    TIMER,
    EVENTFD,
    INODE,
    SIGNAL
};

typedef struct frixia_file_descriptor
{
    int fd;
    enum FrixiaFDType type;
    frixia_fd_args_t  *type_data;

} frixia_file_descriptor_t;

typedef struct convoy
{
    int maximum_size;
    int size;
    frixia_file_descriptor_t filedescriptors[25];

    pthread_mutex_t *mutex;

} convoy_t;

convoy_t *convoy_create(int dim);
void convoy_destroy(convoy_t *convoy);
void convoy_add_tcp_filedescriptor(convoy_t *c, int fd, const char *ip, int port, int bytes);
void convoy_add_udp_filedescriptor(convoy_t *c, int fd, const char *ip, int port, int bytes);
void convoy_add_fifo_filedescriptor(convoy_t *c, int fd, const char *path, int bytes);
void convoy_add_timer_filedescriptor(convoy_t *c, int fd, int delay, int interval);
void convoy_add_inode_filedescriptor(convoy_t *c, int fd, char *filepath);
void convoy_add_signal_filedescriptor(convoy_t *c, int fd,FRIXIA_SIGNAL sig);
void convoy_add_eventfd_filedescriptor(convoy_t *c, int fd);

void convoy_copy_fd(convoy_t *c, int source_fd, int destination_fd);
void convoy_remove_fd(convoy_t *c, int fd);

// void convoy_register_http_method_callback(convoy_t *c,const char *ip, int port, const char *method,const char *path,void *(*fun)(void *), void *arg);
// void convoy_register_fins_command_callback(convoy_t *c, enum FrixiaFDType type, const char *ip, int port, uint8_t first, uint8_t second,void *(*fun)(void *), void *arg);
// void convoy_register_noprotocol_callback(convoy_t *c, enum FrixiaFDType type, const char *ip, int port, void *(*fun)(void *), void *arg);
// void convoy_register_noprotocol_tcp_callback(convoy_t *convoy,const char *ip,int port, void *(*fun)(void *), void *arg);

//void convoy_register_timer_callback(convoy_t *c, const char *id, void *fun, void *arg);

#endif