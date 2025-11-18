#ifndef FRIXIA_TCP_H
#define FRIXIA_TCP_H

#include "frixia_tcp_codes.h"

int start_tcp_listening(const char *ip, int port);
int stop_tcp_listening(int fd);
int read_tcp(int fd,char buf[], int size, int *error);

int write_tcp( int client_fd,char buffer[],int size);
int accept_tcp( int fd, int *reply_fd);
char* get_ftcp_code_string(FTCP_CODE_T c);
int get_ftcp_code_string_from_string(char *s);
int close_tcp(int fd);


#endif