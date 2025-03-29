#include "frixia_fd_args.h"


void set_frixia_tcp_fd(frixia_fd_args_t *fd, const char *ip, int port, int read_size)
{
    fd->tcp_info->ip = ip;
    fd->tcp_info->port = port;
    fd->tcp_info->read_size = read_size;
}

void set_frixia_udp_fd(frixia_fd_args_t *fd, const char *ip, int port, int read_size)
{
    fd->udp_info->ip = ip;
    fd->udp_info->port = port;
    fd->udp_info->read_size = read_size;
}

void set_frixia_fifo_fd(frixia_fd_args_t *fd, const char *name, int read_size)
{
    fd->fifo_info->name = name;
    fd->fifo_info->read_size = read_size;
}

void set_frixia_timer_fd(frixia_fd_args_t *fd, const char *id, int delay, int interval)
{
    fd->timer_info->id = id;
    fd->timer_info->delay = delay;
    fd->timer_info->interval = interval;
}

void set_frixia_scheduler_fd(frixia_fd_args_t *fd, int tick)
{
    fd->scheduler_info->tick = tick;
}

void set_frixia_eventfd_fd(frixia_fd_args_t *fd, int ev_fd)
{
    fd->eventfd_info->fd = ev_fd;
}

void set_frixia_file_modify_fd(frixia_fd_args_t *fd, char *dir, char *name)
{
    fd->file_modify_info->directory = dir;
    fd->file_modify_info->filename = name;
}
void set_frixia_file_creation_fd(frixia_fd_args_t *fd, char *dir, char *name)
{
    fd->file_creation_info->directory = dir;
    fd->file_creation_info->filename = name;
}

