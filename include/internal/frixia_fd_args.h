#ifndef FRIXIA_FD_ARGS_H
#define FRIXIA_FD_ARGS_H

typedef struct frixia_tcp
{
    const char *ip;
    int         port;
    int         read_size;

} frixia_tcp_t;
typedef struct frixia_udp
{
    const char *ip;
    int         port;
    int         read_size;

} frixia_udp_t;
typedef struct frixia_fifo
{
    const char *name;
    int         read_size;

} frixia_fifo_t;
typedef struct frixia_timer
{
    int         delay;
    int         interval;

} frixia_timer_t;

typedef struct frixia_scheduler
{
    int         tick;

} frixia_scheduler_t;

typedef struct frixia_eventfd
{
    int         fd;

} frixia_eventfd_t;

typedef struct frixia_inode
{
    char *path;

}frixia_inode_t;

typedef union frixia_fd_args
{
    frixia_tcp_t tcp_info;
    frixia_udp_t udp_info;
    frixia_fifo_t fifo_info;
    frixia_timer_t timer_info;
    frixia_scheduler_t scheduler_info;
    frixia_eventfd_t eventfd_info;
    frixia_inode_t inode_info;
    
} frixia_fd_args_t;

void set_frixia_tcp_fd(frixia_fd_args_t *fd, const char *ip, int port, int read_size);
void set_frixia_udp_fd(frixia_fd_args_t *fd, const char *ip, int port, int read_size);
void set_frixia_fifo_fd(frixia_fd_args_t *fd, const char *name, int read_size);
void set_frixia_timer_fd(frixia_fd_args_t *fd, int delay, int interval);
void set_frixia_scheduler_fd(frixia_fd_args_t *fd, int tick);
void set_frixia_eventfd_fd(frixia_fd_args_t *fd, int ev_fd);
void set_frixia_inode_fd(frixia_fd_args_t *fd, char *filepath);

#endif