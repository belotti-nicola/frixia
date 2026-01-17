#ifndef FINODE_HANDLER_H
#define FINODE_HANDLER_H
#include <frixia/frixia_inode.h>

int start_inode_listening(const char *path, FRIXIA_INODE_FLAG mask);
int stop_inode_listening(int fd);
int read_inode(int fd,char buf[],int max_size);
#endif