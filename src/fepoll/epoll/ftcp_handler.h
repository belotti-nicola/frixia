#ifndef FTCP_HANDLER_H
#define FTCP_HANDLER_H

#include <frixia/frixia_tcp.h>

FRIXIA_TCP_FD_RESULT start_tcp_listening(const char *ip, int port);
FRIXIA_TCP_RESULT stop_tcp_listening(int fd);
FRIXIA_TCP_READ_RESULT read_tcp(int fd,char buf[], int size);
FRIXIA_TCP_WRITE_RESULT write_tcp( int client_fd,char buffer[],int size);
FRIXIA_TCP_RESULT accept_tcp( int fd, int *reply_fd);
FRIXIA_TCP_RESULT close_tcp(int fd);


#endif