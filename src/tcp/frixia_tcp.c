#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "frixia_tcp.h"
#include "../core/frixia_codes.h"

int start_tcp_listening(int epoll_fd,
                        int port)
{
    printf("start_tcp_listening %d %d\n",epoll_fd,port);
    if (epoll_fd <= 0)
    {
        return ERR_FTCP_START_MALFORMED_EPOLL_FD;
    }
    if (port < 0 || port > 65535)
    {
        return ERR_FTCP_START_MALFORMED_PORT;
    }
    
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1)
    {
        return ERR_FTCP_SOCKET;
    }

    int reuse = 1;
    if (setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0)
    {
        return ERR_FTCP_SETSOCKETOPT;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);
    int retVal = bind(tcp_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retVal < 0)
    {
        return ERR_FTCP_BIND;
    }

    if (listen(tcp_fd, 10) == -1)
    {
        return ERR_FTCP_LISTEN;
    }
    struct epoll_event ev_tcp;
    ev_tcp.events = EPOLLIN | EPOLLET;
    ev_tcp.data.fd = tcp_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_fd, &ev_tcp) < 0)
    {
        return ERR_FTCP_START_EPOLLCTL_ADD;
    }
    
    
    return tcp_fd;
}

int stop_tcp_listening(int epoll_fd,
                       int closing_fd)
{
    if(epoll_fd <= 0)
    {
        return ERR_FTCP_STOP_MALFORMED_EPOLL_FD;
    }
    if(closing_fd <= 0)
    {
        return ERR_FTCP_STOP_MALFORMED_TARGET_FD;
    }
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, closing_fd, NULL);
    if (epoll_ctl_retval == -1)
    {
        return ERR_FTCP_STOP_EPOLLCTL_DEL;
    }
    close(closing_fd);
    
    return FTCP_OK;
}

int read_tcp(int filedescriptor,char buf[], int size, int* reply_fd)
{
    struct sockaddr in_addr;
    socklen_t in_len;

    in_len = sizeof(in_addr);
    *reply_fd = accept(filedescriptor, &in_addr, &in_len);
    if (reply_fd == -1)
    {
        return ERR_FTCP_ACCEPTING;
    }
    int read_bytes = read(reply_fd, buf, size);
    if (read_bytes < 0)
    {
        close(reply_fd);
        return ERR_FTCP_READING;
    }
    return FTCP_OK;
}

int write_tcp( int reply_fd,char buffer[],int size )
{
    int ret_code = write(reply_fd, buffer, size);
    printf("PORCHIDDIO::%d(%d)\n",ret_code,reply_fd);
    if(ret_code == -1)
    {
        close(reply_fd);
        return ERR_FTCP_WRITE;
    }
    close(reply_fd);
    return FTCP_OK;
}

char* get_ftcp_code_string(enum FTCP_CODE c){
    switch(c)
    {
        case FTCP_OK:                           return "FTCP_OK";
        case ERR_FTCP_SOCKET:                   return "ERR_FTCP_SOCKET";
        case ERR_FTCP_SETSOCKETOPT:             return "ERR_FTCP_SETSOCKETOPT";
        case ERR_FTCP_BIND:                     return "ERR_FTCP_BIND";
        case ERR_FTCP_LISTEN:                   return "ERR_FTCP_LISTEN";
        case ERR_FTCP_START_EPOLLCTL_ADD:       return "ERR_FTCP_START_EPOLLCTL_ADD";
        case ERR_FTCP_STOP_EPOLLCTL_DEL:        return "ERR_FTCP_STOP_EPOLLCTL_DEL";
        case ERR_FTCP_ACCEPTING:                return "ERR_FTCP_ACCEPTING";
        case ERR_FTCP_READING:                  return "ERR_FTCP_READING";
        case ERR_FTCP_START_MALFORMED_EPOLL_FD: return "ERR_FTCP_START_MALFORMED_EPOLL_FD";
        case ERR_FTCP_START_MALFORMED_PORT:     return "ERR_FTCP_START_MALFORMED_PORT";
        case ERR_FTCP_STOP_MALFORMED_EPOLL_FD:  return "ERR_FTCP_STOP_MALFORMED_EPOLL_FD";
        case ERR_FTCP_STOP_MALFORMED_TARGET_FD: return "ERR_FTCP_STOP_MALFORMED_TARGET_FD";
        case ERR_FTCP_WRITE:                    return "ERR_FTCP_WRITE";
        default: 
        {
            printf("get_ftcp_code_string: %d\n",c);
            return "UNKNOWN CODE";
        }
    }
}
int get_ftcp_code_string_from_string(char *s){
    if(strcmp("FTCP_OK",s) == 0)                                 { return FTCP_OK; }
    else if(strcmp("ERR_FTCP_SOCKET",s) == 0)                    { return ERR_FTCP_SOCKET;}
    else if(strcmp("ERR_FTCP_SETSOCKETOPT",s) == 0)              { return ERR_FTCP_SETSOCKETOPT;}
    else if(strcmp("ERR_FTCP_BIND",s) == 0)                      { return ERR_FTCP_BIND;}
    else if(strcmp("ERR_FTCP_LISTEN",s) == 0)                    { return ERR_FTCP_LISTEN;}
    else if(strcmp("ERR_FTCP_START_EPOLLCTL_ADD",s) == 0)        { return ERR_FTCP_START_EPOLLCTL_ADD;}
    else if(strcmp("ERR_FTCP_STOP_EPOLLCTL_DEL",s) == 0)         { return ERR_FTCP_STOP_EPOLLCTL_DEL;}
    else if(strcmp("ERR_FTCP_ACCEPTING",s) == 0)                 { return ERR_FTCP_ACCEPTING;}
    else if(strcmp("ERR_FTCP_READING",s) == 0)                   { return ERR_FTCP_READING;}
    else if(strcmp("ERR_FTCP_START_MALFORMED_EPOLL_FD",s) == 0)  { return ERR_FTCP_READING;}
    else if(strcmp("ERR_FTCP_START_MALFORMED_PORT",s) == 0)      { return ERR_FTCP_READING;}
    else if(strcmp("ERR_FTCP_STOP_MALFORMED_EPOLL_FD",s) == 0)   { return ERR_FTCP_READING;}
    else if(strcmp("ERR_FTCP_STOP_MALFORMED_TARGET_FD",s) == 0)  { return ERR_FTCP_READING;}
    else if(strcmp("ERR_FTCP_WRITE",s)==0)                       { return ERR_FTCP_WRITE; }
    else 
    {
        printf("get_ftcp_code_string: unknown string %s\n",s);
        return -1;
    }
}