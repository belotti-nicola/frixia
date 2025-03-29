#include "frixia_inode.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

int start_inode_listening(char *path)
{
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        printf("inotify_init :: errno %d\n",errno);
        return FERR_START_INODE_INOTIFY_INIT;
    }

    int wd = inotify_add_watch(inotify_fd, path, IN_CREATE );
    if (wd == -1) {
        printf("inotify_init :: %s(errno %d)\n",path,errno);
        return FERR_START_INODE_INOTIFY_ADD_WATCHDOG;
    }

    printf("start_INODE_listening: fd %d, path %s\n",inotify_fd,path);
    return inotify_fd;
}
int stop_INODE_listening(int fd)
{
    int ret = close(fd);
    if(ret < 0)
    {
        printf("Error::start_INODE_listening::inotify_add_watch :: errno %d\n",errno);
        return FERR_STOP_INODE_LISTENING;
    }
    return 0;
    
}
int read_INODE(int fd,char *buf,int max_size)
{
    int bytes_read = read(fd, buf, max_size);
    if (bytes_read < 0)
    {
        printf("start_INODE_listening :: read_INODE :: errno %d\n",errno);
        return FERR_READ_INODE; 
    }
    return bytes_read;
}
