#ifndef FRIXIA_INODE_H
#define FRIXIA_INODE_H

#include "frixia_inode_flags.h"

typedef enum FINODE_T
{
    FERR_START_INODE_INOTIFY_INIT=-1,
    FERR_START_INODE_INOTIFY_ADD_WATCHDOG=-2,
    FERR_STOP_INODE_LISTENING=-3,
    FERR_READ_INODE=-4

} FINODE_T;


int start_inode_listening(char *path, FRIXIA_INODE_FLAG_T mask);
int stop_inode_listening(int fd);
int read_inode(int fd,char *buf,int max_size);
#endif