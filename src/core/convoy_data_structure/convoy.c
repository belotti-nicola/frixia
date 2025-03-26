#include <stdio.h>
#include "frixia_fd_args.h"

#include "convoy.h"

void convoy_add_tcp_filedescriptor(convoy_t *c,int fd, const char *ip,int port,int bytes)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = TCP;
        set_frixia_tcp_fd(c->filedescriptors[0].type_data,ip,port,bytes);
        c->size++;
        return;
    }

    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == TCP &&
            c->filedescriptors[i].type_data->tcp_info->ip == ip &&
            c->filedescriptors[i].type_data->tcp_info->port == port)
        {
            printf("Convoy error: tcp socket is already present (%s %d).\n",ip,port);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = TCP;
    set_frixia_tcp_fd(c->filedescriptors[target].type_data,ip,port,bytes);
}
void convoy_add_udp_filedescriptor(convoy_t *c,int fd,const char *ip,int port,int bytes)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = UDP;
        set_frixia_udp_fd(c->filedescriptors[0].type_data,ip,port,bytes);
        c->size++;
        return;
    }

    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == UDP &&
            c->filedescriptors[i].type_data->udp_info->ip == ip &&
            c->filedescriptors[i].type_data->udp_info->port == port)
        {
            printf("Convoy error: udp socket is already present (%s %d).\n",ip,port);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = UDP;
    set_frixia_udp_fd(c->filedescriptors[target].type_data,ip,port,bytes);
    c->size++;
}
void convoy_add_fifo_filedescriptor(convoy_t *c,int fd,const char *path,int bytes)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = FIFO;
        set_frixia_fifo_fd(c->filedescriptors[0].type_data,path,bytes);
        c->size++;
        return;
    }

    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == FIFO &&
            c->filedescriptors[i].type_data->fifo_info->name== path)
        {
            printf("Convoy error: fifo socket is already present (%s).\n",path);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = FIFO;
    set_frixia_fifo_fd(c->filedescriptors[target].type_data,path,bytes);
    c->size++;
}
void convoy_add_timer_filedescriptor(convoy_t *c,int fd,const char *id, int delay, int interval)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = TIMER;
        set_frixia_timer_fd(c->filedescriptors[0].type_data,id,delay,interval);
        c->size++;
        return;
    }
    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == TIMER &&
            c->filedescriptors[i].type_data->timer_info->delay == delay &&
            c->filedescriptors[i].type_data->timer_info->interval == interval )
        {
            printf("Convoy error: timer socket is already present (%d %d).\n",delay,interval);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = TIMER;
    set_frixia_timer_fd(c->filedescriptors[target].type_data,id,delay,interval);
    c->size++;
}
void convoy_add_scheduler_filedescriptor(convoy_t *c, int fd, int tick)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = SCHEDULER;
        c->size++;
        return;
    }
    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == SCHEDULER)
        {
            printf("Convoy error: scheduler is already present (%d).\n",tick);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = SCHEDULER;
    set_frixia_scheduler_fd(c->filedescriptors[target].type_data,tick);
    c->size++;
}
void convoy_add_scheduled_timer_filedescriptor(convoy_t *c,int fd)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = EVENTFD;
        c->size++;
        return;
    }
    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == EVENTFD && c->filedescriptors[i].type_data->eventfd_info->fd == fd )
        {
            printf("Convoy error: eventfd is already present (%d).\n",fd);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = EVENTFD;
    set_frixia_eventfd_fd(c->filedescriptors[target].type_data,fd);
    c->size++;
}


void convoy_register_http_callback(convoy_t *c,const char *method,const char *path,void *fun, void *arg)
{}
void convoy_register_fins_callback(convoy_t *c)
{}
void convoy_register_timer_callback(convoy_t *c,const char *id,void *fun,void *arg)
{}                                            
