#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "frixia_tcp.h"
#include "frixia_tcp_fd_result.h"
#include "frixia_tcp_read_result.h"
#include "../../../../core/frixia_codes.h"
#include "../../../../core/frixia_h.h"
#include "../../../../utils/networking/network.h"
#include <fcntl.h>
#include <arpa/inet.h>

FRIXIA_TCP_FD_RESULT start_tcp_listening(const char *s, int port)
{
    if (port < 0 || port > 65535)
    {
        return  ftcp_create_fd_result(-1,ERR_FTCP_START_MALFORMED_PORT,errno);
    }
    
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_fd == -1)
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_SOCKET,errno);
    }

    int reuse = 1;
    if (setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0)
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_SETSOCKETOPT,errno);
    }
    if ( fcntl(tcp_fd, F_SETFL, fcntl(tcp_fd, F_GETFL, 0) | O_NONBLOCK) < 0 )
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_FNCTL,errno);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    
    if ( is_valid_ipv4(s) == false )
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_IP_NOT_VALID,errno);
    }

    if ( inet_pton(AF_INET, s, &serveraddr.sin_addr) < 0 )
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_INET_PTON,errno);
    } 
    serveraddr.sin_port = htons(port);
    int retVal = bind(tcp_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retVal < 0)
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_BIND,errno);
    }

    if (listen(tcp_fd, 10) == -1)
    {
        return ftcp_create_fd_result(-1,ERR_FTCP_LISTEN,errno);
    }   
    
    return ftcp_create_fd_result(tcp_fd,FTCP_OK,-1);
}

FRIXIA_TCP_RESULT stop_tcp_listening(int closing_fd)
{
    int retVal = close(closing_fd);
    if (retVal != 0 )
    {
        return ftcp_create_tcp_result(FTCP_CLOSE_ERROR,errno);
    }

    return ftcp_create_tcp_result(FTCP_OK,-1);
}

FRIXIA_TCP_RESULT accept_tcp(int filedescriptor, int *reply)
{
    struct sockaddr in_addr;
    socklen_t in_len;

    in_len = sizeof(in_addr);
    
    int fd = accept(filedescriptor, &in_addr, &in_len);
    if ( fd == -1 )
    {
        return ftcp_create_tcp_result(ERR_FTCP_ACCEPTING,errno);
    }

    int flags = fcntl(fd, F_GETFL, 0);
    if ( fcntl(fd, F_SETFL, flags | O_NONBLOCK) )
    {
        return ftcp_create_tcp_result(ERR_FTCP_ACCEPTING,errno);
    }
    *reply = fd;
    return ftcp_create_tcp_result(FTCP_OK,errno);
}

FRIXIA_TCP_READ_RESULT read_tcp(int filedescriptor,char buf[], int size)
{   
    int read_bytes = read(filedescriptor, buf, size);
    if (read_bytes < 0)
    {
        return frixia_tcp_read_create(ERR_FTCP_READING,read_bytes,errno);
    }
    return frixia_tcp_read_create(FTCP_OK,read_bytes,-1);
}

FRIXIA_TCP_WRITE_RESULT write_tcp( int reply_fd,char buffer[],int size )
{
    int ret_code = write(reply_fd, buffer, size);
    if(ret_code == -1)
    {
        close(reply_fd);
        return frixia_tcp_write_create(ERR_FTCP_WRITE,errno);
    }
    
    return frixia_tcp_write_create(FTCP_OK,-1);
}

FRIXIA_TCP_RESULT close_tcp(int fd)
{
    int ret_code = close(fd);
    if ( ret_code < 0)
    {
        return ftcp_create_tcp_result(FTCP_CLOSE_ERROR,errno);
    }

    return ftcp_create_tcp_result(FTCP_OK,-1);
}
