#include <stdio.h>
#include "frixia_fd_args.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <frixia/frixia_inode.h>
#include <internal/convoy.h>

void convoy_add_tcp_filedescriptor(convoy_t *c, int fd, const char *ip, int port, int bytes)
{
    pthread_mutex_lock(c->mutex);
    int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = TCP;
    set_frixia_tcp_fd(c->filedescriptors[fd].type_data,ip,port,bytes);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}
void convoy_add_udp_filedescriptor(convoy_t *c,int fd,const char *ip,int port,int bytes)
{
        int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = UDP;
    set_frixia_udp_fd(c->filedescriptors[fd].type_data,ip,port,bytes);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}
void convoy_add_fifo_filedescriptor(convoy_t *c,int fd,const char *path,int bytes)
{
    pthread_mutex_lock(c->mutex);
    int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = FIFO;
    set_frixia_fifo_fd(c->filedescriptors[fd].type_data,path,bytes);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}
void convoy_add_timer_filedescriptor(convoy_t *c,int fd, int delay, int interval)
{
    pthread_mutex_lock(c->mutex);
    int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = FIFO;
    set_frixia_timer_fd(c->filedescriptors[fd].type_data,delay,interval);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}

void convoy_add_inode_filedescriptor(convoy_t *c, int fd, char *filepath,FRIXIA_INODE_FLAG mask)
{
    pthread_mutex_lock(c->mutex);
    int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = INODE;
    set_frixia_inode_fd(c->filedescriptors[fd].type_data,filepath,mask);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}
void convoy_add_signal_filedescriptor(convoy_t *c, int fd,FRIXIA_SIGNAL sig)
{
    pthread_mutex_lock(c->mutex);
    int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = SIGNAL;
    set_frixia_signal_fd(c->filedescriptors[fd].type_data,sig);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}

void convoy_add_eventfd_filedescriptor(convoy_t *c, int fd)
{
    pthread_mutex_lock(c->mutex);
    int dim = c->maximum_size;
    if( c->size == dim)
    {
        printf("Convoy reached maximum size.\n");
        pthread_mutex_unlock(c->mutex);
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = EVENTFD;
    set_frixia_eventfd_fd(c->filedescriptors[fd].type_data,fd);
    c->size = c->size +1;
    pthread_mutex_unlock(c->mutex);
}
                                     
void convoy_copy_fd(convoy_t *c, int source_fd, int destination_fd)
{
    c->filedescriptors[destination_fd].fd        = destination_fd;
    c->filedescriptors[destination_fd].type      = c->filedescriptors[source_fd].type;
    c->filedescriptors[destination_fd].type_data = c->filedescriptors[source_fd].type_data;   
}

void convoy_remove_fd(convoy_t *c, int fd)
{
    c->filedescriptors[fd].fd        = -1;
    c->filedescriptors[fd].type      = UNDEFINED;
}

convoy_t *convoy_create(int maximum_size)
{
    convoy_t *retVal = malloc(sizeof(convoy_t));
    if( retVal == NULL )
    {
        printf("Error creating convoy!\n");
        return NULL;
    }

    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    if ( mutex == NULL )
    {
        printf("Error creating convoy mutex!\n");
        return NULL;
    }
    pthread_mutex_init(mutex,NULL);

    retVal->mutex = mutex;
    retVal->size = 0;
    retVal->maximum_size = maximum_size;

    
    frixia_file_descriptor_t *ptr = malloc(maximum_size * sizeof(frixia_file_descriptor_t));
    if ( ptr == NULL )
    {
        printf("Error creating frixia_file_descriptors\n");
        return NULL;
    }
    for(int i=0;i<maximum_size;i++)
    {
        ptr[i].fd = 0;
        ptr[i].type = UNDEFINED;
        frixia_fd_args_t *type_data = malloc(sizeof(frixia_fd_args_t));
        frixia_tcp_t tcp_info = {
        .ip = "0.0.0.0",
        .port = 9600,
        .read_size = 0
        };
        type_data->tcp_info = tcp_info;
        ptr[i].type_data = type_data;
    }
    for(int i=0;i<maximum_size;i++)
    {
        retVal->filedescriptors[i] = ptr[i];
    }


    return retVal;
}

void convoy_destroy(convoy_t *convoy)
{
    free(convoy->mutex);
    free(convoy);
}