#include <stdio.h>
#include "frixia_fd_args.h"
#include "frixia_callback.h"
#include "../../utils/datastructures/simple_hash_map/simple_hash_map.h"
#include <string.h>

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
        c->size = 1 ;
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
    c->size = c->size +1;
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
        c->size = 1;
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
    frixia_fd_args_t *args = c->filedescriptors[target].type_data;
    set_frixia_udp_fd(args,ip,port,bytes);
    c->size = c->size +1;
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
        c->size = 1;
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
    c->size = c->size +1;
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
        c->size = 1;
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
    c->size = c->size +1;
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
    c->size = c->size +1;
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
        c->size = 1;
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
    c->size = c->size +1;
}
void convoy_add_add_inode_filedescriptor(convoy_t *c,int fd, char *path)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }
    if( c->size == 0)
    {
        c->filedescriptors[0].fd   = fd;
        c->filedescriptors[0].type = INODE;
        set_frixia_inode_fd(c->filedescriptors[0].type_data,path);
        c->size = 1;
        return;
    }
    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        if( c->filedescriptors[i].type == INODE && c->filedescriptors[i].type_data->inode_info->path ==  path )
        {
            printf("Convoy error: eventfd is already present (%d).\n",fd);
            return;
        }
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = EVENTFD;
    set_frixia_inode_fd(c->filedescriptors[target].type_data,path);
    c->size = c->size +1;
}

void convoy_register_http_callback(convoy_t *c,const char *ip, int port, const char *method,const char *path,void *(*fun)(void *), void *arg)
{
    int index = -1;
    int size  = c->size;
    for(int i=0; i<size; i++)
    {
        frixia_file_descriptor_t fd = c->filedescriptors[i];
        if ( fd.type != TCP )
        {
            continue;
        }
        frixia_tcp_t *tcp_info = fd.type_data->tcp_info;
        if( strcmp(tcp_info->ip,ip) == 0 &&
            tcp_info->port == port )
        {
            index = i;
        }
        printf("Entry found::%s %d %d\n",tcp_info->ip,tcp_info->port,tcp_info->read_size);
    }
    if( index == -1 )
    {
        printf("HTTP Entry not present! %s %d\n",ip,port);
        return;
    }
    printf("index %d\n",index);

    c->filedescriptors[index].protocol = HTTP;

    frixia_callback_t cb = create_frixia_callback(fun,arg);
    HashEntry_t *he = create_hash_entry(method,&cb);  
    HashMap_t *hm;
    void *ptr = c->filedescriptors[index].protocol_data;
    if( ptr == NULL )
    {
        printf("Creating HTTP structure\n");
        hm = create_hash_map(16);
        c->filedescriptors[size].protocol_data = hm;
    }
    else 
    {
        printf("Adding to HTTP structure\n");
        hm = (HashMap_t *)c->filedescriptors[size].protocol_data;
    }
    add_entry(hm,he);
}

void convoy_register_fins_callback(convoy_t *c)
{}
void convoy_register_timer_callback(convoy_t *c,const char *id,void *fun,void *arg)
{}                                            
