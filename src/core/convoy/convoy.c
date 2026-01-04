#include <stdio.h>
#include "frixia_fd_args.h"
#include "frixia_callback.h"
#include "../callback_suite/callback_data/frixia_http_key.h"
#include "../../utils/datastructures/simple_hash_map/simple_hash_map.h"
#include "../callback_suite/callback_data/frixia_fins_key.h"

#include <stdint.h>
#include <string.h>

#include "convoy.h"

void convoy_add_tcp_filedescriptor(convoy_t *c, int fd, const char *ip, int port, int bytes,FRIXIA_SUPPORTED_PROTOCOL_T protocol)
{
    if( c->size == MAXIMUM_FD_NUMBER)
    {
        printf("Convoy reached maximum size.\n");
        return;
    }

    c->filedescriptors[fd].fd   = fd;
    c->filedescriptors[fd].type = TCP;
    c->filedescriptors[fd].protocol = protocol;
    set_frixia_tcp_fd(c->filedescriptors[fd].type_data,ip,port,bytes);
    c->size = c->size +1;
}
void convoy_add_udp_filedescriptor(convoy_t *c,int fd,const char *ip,int port,int bytes)
{
    // if( c->size == MAXIMUM_FD_NUMBER)
    // {
    //     printf("Convoy reached maximum size.\n");
    //     return;
    // }
    // if( c->size == 0)
    // {
    //     c->filedescriptors[0].fd   = fd;
    //     c->filedescriptors[0].type = UDP;
    //     set_frixia_udp_fd(c->filedescriptors[0].type_data,ip,port,bytes);
    //     c->size = 1;
    //     return;
    // }

    
    // int target = c->size;
    // for(int i=0;i<target;i++)
    // {
    //     if( c->filedescriptors[i].type == UDP &&
    //         c->filedescriptors[i].type_data->udp_info->ip == ip &&
    //         c->filedescriptors[i].type_data->udp_info->port == port)
    //     {
    //         printf("Convoy error: udp socket is already present (%s %d).\n",ip,port);
    //         return;
    //     }
    // }
    // c->filedescriptors[target].fd   = fd;
    // c->filedescriptors[target].type = UDP;
    // frixia_fd_args_t *args = c->filedescriptors[target].type_data;
    // set_frixia_udp_fd(args,ip,port,bytes);
    // c->size = c->size +1;
}
void convoy_add_fifo_filedescriptor(convoy_t *c,int fd,const char *path,int bytes)
{
//     if( c->size == MAXIMUM_FD_NUMBER)
//     {
//         printf("Convoy reached maximum size.\n");
//         return;
//     }
//     if( c->size == 0)
//     {
//         c->filedescriptors[0].fd   = fd;
//         c->filedescriptors[0].type = FIFO;
//         set_frixia_fifo_fd(c->filedescriptors[0].type_data,path,bytes);
//         c->size = 1;
//         return;
//     }

    
//     int target = c->size;
//     for(int i=0;i<target;i++)
//     {
//         if( c->filedescriptors[i].type == FIFO &&
//             c->filedescriptors[i].type_data->fifo_info->name== path)
//         {
//             printf("Convoy error: fifo socket is already present (%s).\n",path);
//             return;
//         }
//     }
//     c->filedescriptors[target].fd   = fd;
//     c->filedescriptors[target].type = FIFO;
//     set_frixia_fifo_fd(c->filedescriptors[target].type_data,path,bytes);
//     c->size = c->size +1;
}
void convoy_add_timer_filedescriptor(convoy_t *c,int fd,const char *id, int delay, int interval)
{
    // if( c->size == MAXIMUM_FD_NUMBER)
    // {
    //     printf("Convoy reached maximum size.\n");
    //     return;
    // }
    // if( c->size == 0)
    // {
    //     c->filedescriptors[0].fd   = fd;
    //     c->filedescriptors[0].type = TIMER;
    //     set_frixia_timer_fd(c->filedescriptors[0].type_data,id,delay,interval);
    //     c->size = 1;
    //     return;
    // }
    
    // int target = c->size;
    // for(int i=0;i<target;i++)
    // {
    //     if( c->filedescriptors[i].type == TIMER &&
    //         c->filedescriptors[i].type_data->timer_info->delay == delay &&
    //         c->filedescriptors[i].type_data->timer_info->interval == interval )
    //     {
    //         printf("Convoy error: timer socket is already present (%d %d).\n",delay,interval);
    //         return;
    //     }
    // }
    // c->filedescriptors[target].fd   = fd;
    // c->filedescriptors[target].type = TIMER;
    // set_frixia_timer_fd(c->filedescriptors[target].type_data,id,delay,interval);
    // c->size = c->size +1;
}
void convoy_add_scheduler_filedescriptor(convoy_t *c, int fd, int tick)
{
    // if( c->size == MAXIMUM_FD_NUMBER)
    // {
    //     printf("Convoy reached maximum size.\n");
    //     return;
    // }
    // if( c->size == 0)
    // {
    //     c->filedescriptors[0].fd   = fd;
    //     c->filedescriptors[0].type = SCHEDULER;
    //     c->size++;
    //     return;
    // }
    
    // int target = c->size;
    // for(int i=0;i<target;i++)
    // {
    //     if( c->filedescriptors[i].type == SCHEDULER)
    //     {
    //         printf("Convoy error: scheduler is already present (%d).\n",tick);
    //         return;
    //     }
    // }
    // c->filedescriptors[target].fd   = fd;
    // c->filedescriptors[target].type = SCHEDULER;
    // set_frixia_scheduler_fd(c->filedescriptors[target].type_data,tick);
    // c->size = c->size +1;
}
void convoy_add_scheduled_timer_filedescriptor(convoy_t *c,int fd)
{
//     if( c->size == MAXIMUM_FD_NUMBER)
//     {
//         printf("Convoy reached maximum size.\n");
//         return;
//     }
//     if( c->size == 0)
//     {
//         c->filedescriptors[0].fd   = fd;
//         c->filedescriptors[0].type = EVENTFD;
//         c->size = 1;
//         return;
//     }
    
//     int target = c->size;
//     for(int i=0;i<target;i++)
//     {
//         if( c->filedescriptors[i].type == EVENTFD && c->filedescriptors[i].type_data->eventfd_info->fd == fd )
//         {
//             printf("Convoy error: eventfd is already present (%d).\n",fd);
//             return;
//         }
//     }
//     c->filedescriptors[target].fd   = fd;
//     c->filedescriptors[target].type = EVENTFD;
//     set_frixia_eventfd_fd(c->filedescriptors[target].type_data,fd);
//     c->size = c->size +1;
 }
void convoy_add_add_inode_filedescriptor(convoy_t *c,int fd, char *path)
{
    // if( c->size == MAXIMUM_FD_NUMBER)
    // {
    //     printf("Convoy reached maximum size.\n");
    //     return;
    // }
    // if( c->size == 0)
    // {
    //     c->filedescriptors[0].fd   = fd;
    //     c->filedescriptors[0].type = INODE;
    //     set_frixia_inode_fd(c->filedescriptors[0].type_data,path);
    //     c->size = 1;
    //     return;
    // }
    
    // int target = c->size;
    // for(int i=0;i<target;i++)
    // {
    //     if( c->filedescriptors[i].type == INODE && c->filedescriptors[i].type_data->inode_info->path ==  path )
    //     {
    //         printf("Convoy error: eventfd is already present (%d).\n",fd);
    //         return;
    //     }
    // }
    // c->filedescriptors[target].fd   = fd;
    // c->filedescriptors[target].type = EVENTFD;
    // set_frixia_inode_fd(c->filedescriptors[target].type_data,path);
    // c->size = c->size +1;
}

// void convoy_register_http_method_callback(convoy_t *c,const char *ip, int port, const char *method,const char *path,void *(*fun)(void *), void *arg)
// {
//     int index = -1;
//     int size  = c->size;
//     for(int i=0; i<size; i++)
//     {
//         frixia_file_descriptor_t fd = c->filedescriptors[i];
//         if ( fd.type != TCP )
//         {
//             continue;
//         }
//         frixia_tcp_t tcp_info = fd.type_data->tcp_info;
//         if( strcmp(tcp_info.ip,ip) == 0 &&
//             tcp_info.port == port )
//         {
//             index = i;
//             printf("Entry found::%s %d %d\n",tcp_info->ip,tcp_info->port,tcp_info->read_size);
//             break;
//         }
//     }
//     if( index == -1 )
//     {
//         printf("HTTP Entry not present! %s %d\n",ip,port);
//         return;
//     }
//     printf("index %d\n",index);

//     c->filedescriptors[index].protocol = HTTP;

//     frixia_callback_t *cb = create_frixia_callback(fun,arg);
//     int method_len = strlen(method);
//     int path_len   = strlen(path);
    
//     char *key = calloc(sizeof(char),50);//TODO CHECK IF THE CALLOC IS NECESSARY: WHY NOT URL ONLY
//     frixia_compute_http_key(key,50,method,method_len,path,path_len);
//     HashEntry_t *he = create_hash_entry(key,cb);  
//     void **ptr = c->filedescriptors[index].protocol_data;
//     if( *ptr == NULL )
//     {
//         HashMap_t *hm = create_hash_map(16);
//         add_entry(hm,he);
//         *(c->filedescriptors[index].protocol_data) = hm; 
//         printf("Creating HTTP structure %p\n",hm);
//     }
//     else 
//     {
//         void *hash_map_void = *ptr;
//         HashMap_t *hm = (HashMap_t *) hash_map_void;
//         add_entry(hm,he);
//         printf("Adding to HTTP structure %p\n",hm);

//     }
// }

// void convoy_register_fins_command_callback(convoy_t *c, enum FrixiaFDType type, const char *ip, int port, uint8_t first, uint8_t second,void *(*fun)(void *), void *arg)
// {
//     if ( type != TCP && type != UDP )
//     {
//         printf("Error registering FINS: type can be only TCP or UDP (value is instead: %d)\n",type);
//         return;
//     }

//     int index = -1;
//     int size  = c->size;
//     for(int i=0; i<size; i++)
//     {
//         frixia_file_descriptor_t fd = c->filedescriptors[i];
//         if ( type != fd.type )
//         {
//             continue;
//         }
//         if ( type == TCP )
//         {
//             frixia_tcp_t *tcp_info = fd.type_data->tcp_info;
//             if( strcmp(tcp_info->ip,ip) == 0 &&
//                 tcp_info->port == port )
//             {
//                 index = i;
//                 break;
//             }
//         }
//         if ( type == UDP )
//         {
//             frixia_udp_t *udp_info = fd.type_data->udp_info;
//             if( strcmp(udp_info->ip,ip) == 0 &&
//                 udp_info->port == port )
//             {
//                 index = i;
//                 printf("Entry found::%s %d %d\n",udp_info->ip,udp_info->port,udp_info->read_size);
//                 break;
//             }
//         }      
//     }
//     if( index == -1 )
//     {
//         printf("FINS Entry not present! %s %d %d\n",ip,port,type);
//         return;
//     }

//     c->filedescriptors[index].protocol = FINS;
//     frixia_callback_t *cb = create_frixia_callback(fun,arg);
//     char *key = calloc(sizeof(char),16);
//     frixia_compute_fins_key(key,first,second);
//     HashEntry_t *he = create_hash_entry(key,cb);  
//     void **ptr = c->filedescriptors[index].protocol_data;
//     if( *ptr == NULL )
//     {
//         HashMap_t *hm = create_hash_map(16);
//         add_entry(hm,he);
//         *(c->filedescriptors[index].protocol_data) = hm; 
//         printf("Creating FINS structure %p\n",hm);
//     }
//     else 
//     {
//         void *hash_map_void = *ptr;
//         HashMap_t *hm = (HashMap_t *) hash_map_void;
//         add_entry(hm,he);
//         printf("Adding to FINS structure %p\n",hm);
//     }
// }
// void convoy_register_noprotocol_tcp_callback(convoy_t *convoy,const char *ip,int port, void *(*fun)(void *), void *arg)
// {
//     int index = -1;
//     int size  = convoy->size;
//     for(int i=0; i<size; i++)
//     {
//         frixia_file_descriptor_t fd = convoy->filedescriptors[i];
//         if ( TCP != fd.type )
//         {
//             continue;
//         }
//         frixia_tcp_t tcp_info = fd.type_data->tcp_info;
//         if( strcmp(tcp_info.ip,ip) == 0 &&
//             tcp_info.port == port )
//         {
//             index = i;
//             break;
//         }
//     }
//     if( index == -1 )
//     {
//         printf("NOPROTOCOL TCP Entry not present! %s %d\n",ip,port);
//         return;
//     }

//     convoy->filedescriptors[index].protocol = NO_PROTOCOL;
//     frixia_callback_t *cb = create_frixia_callback(fun,arg);
//     *(convoy->filedescriptors[index].protocol_data) = cb; 
// }

// void convoy_register_timer_callback(convoy_t *c,const char *id,void *fun,void *arg)
// {

// }                                            
void convoy_copy_fd(convoy_t *c, int source_fd, int destination_fd)
{
    c->filedescriptors[destination_fd].fd        = destination_fd;
    c->filedescriptors[destination_fd].type      = c->filedescriptors[source_fd].type;
    c->filedescriptors[destination_fd].type_data = c->filedescriptors[source_fd].type_data;   
}

convoy_t *convoy_create()
{
    convoy_t *retVal = malloc(sizeof(convoy_t));
    if( retVal == NULL )
    {
        printf("Error creating convoy!\n");
        return NULL;
    }

    retVal->size = 0;
    retVal->maximum_size = MAXIMUM_FD_NUMBER;

    
    frixia_file_descriptor_t *ptr = malloc(MAXIMUM_FD_NUMBER * sizeof(frixia_file_descriptor_t));
    if ( ptr == NULL )
    {
        printf("Error creating frixia_file_descriptors\n");
        return NULL;
    }
    for(int i=0;i<MAXIMUM_FD_NUMBER;i++)
    {
        ptr[i].fd = 0;
        ptr[i].type = UNDEFINED;
        ptr[i].protocol = NO_PROTOCOL;
        ptr[i].protocol_data = NULL;
        frixia_fd_args_t *type_data = malloc(sizeof(frixia_fd_args_t));
        frixia_tcp_t tcp_info = {
        .ip = "0.0.0.0",
        .port = 9600,
        .read_size = 0
        };
        type_data->tcp_info = tcp_info;
        ptr[i].type_data = type_data;
    }
    for(int i=0;i<MAXIMUM_FD_NUMBER;i++)
    {
        retVal->filedescriptors[i] = ptr[i];
    }


    return retVal;
}

void convoy_destroy(convoy_t *convoy)
{
    free(convoy);
}