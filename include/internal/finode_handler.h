#ifndef FINODE_HANDLER_H
#define FINODE_HANDLER_H
#include <frixia/frixia_inode.h>

FRIXIA_INODE_FD_RESULT start_inode_listening(const char *path, FRIXIA_INODE_FLAG mask);
FRIXIA_INODE_FD_RESULT stop_inode_listening(int fd);
int read_inode(int fd,char buf[],int max_size);
#endif