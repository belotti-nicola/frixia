#include <stdio.h>


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
        c->size++;
        return;
    }

    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        //TODO CHECK
        printf("Existence check\n");
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = TCP;
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
        c->size++;
        return;
    }

    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        //TODO CHECK
        printf("Existence check\n");
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = UDP;
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
        c->size++;
        return;
    }

    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        //TODO CHECK
        printf("Existence check\n");
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = FIFO;
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
        c->size++;
        return;
    }
    
    int target = c->size;
    for(int i=0;i<target;i++)
    {
        //TODO CHECK
        printf("Existence check\n");
    }
    c->filedescriptors[target].fd   = fd;
    c->filedescriptors[target].type = TIMER;
    c->size++;
}

void convoy_register_http_callback(convoy_t *c,const char *method,const char *path,void *fun, void *arg)
{}
void convoy_register_fins_callback(convoy_t *c)
{}
void convoy_register_timer_callback(convoy_t *c,const char *id,void *fun,void *arg)
{}                                            
