#ifndef FRIXIA_TCP_H
#define FRIXIA_TCP_H

#include "frixia_tcp_codes.h"
#include "frixia_tcp_result.h"

FRIXIA_TCP_RESULT start_tcp_listening(const char *ip, int port);
int stop_tcp_listening(int fd);
int read_tcp(int fd,char buf[], int size, int *error);

int write_tcp( int client_fd,char buffer[],int size);
int accept_tcp( int fd, int *reply_fd);
int close_tcp(int fd);


#endif