#ifndef FRIXIA_UDP_H
#define FRIXIA_UDP_H

#include <netinet/in.h>

#include "frixia_udp_codes.h"
#include "frixia_udp_fd_result.h"

FRIXIA_UDP_FD_RESULT start_udp_listening(int port);
int stop_udp_listening(int target_fd);
int read_udp(int fd, char *buf, int buf_size,struct sockaddr_in *client);
int write_udp(int fd, char buf[],int size,struct sockaddr_in *client);

#endif