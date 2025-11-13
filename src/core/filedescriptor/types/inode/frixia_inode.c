#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "frixia_inode_flags.h"

#include "frixia_inode.h"

int start_inode_listening(char *path)
{
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        printf("inotify_init :: errno %d\n",errno);
        return FERR_START_INODE_INOTIFY_INIT;
    }

    int computed_mask = 0;
    FRIXIA_INODE_FLAG_T mask = FINODE_CREATE | FINODE_MASK_CREATE;
    for( int i=0;i<32;i++)
    {
        FRIXIA_INODE_FLAG_T f = (FRIXIA_INODE_FLAG_T)(1u << i);
        if ( mask & f)
        {
            computed_mask |= FRIXIA_TO_INODE_FLAG(f);
        }
    }

    int wd = inotify_add_watch(inotify_fd, path, computed_mask );
    if (wd == -1) 
    {
        printf("inotify_init :: %s(errno %d)\n",path,errno);
        return FERR_START_INODE_INOTIFY_ADD_WATCHDOG;
    }

    printf("start_inode_listening: fd %d, path %s\n",inotify_fd,path);
    return inotify_fd;
}
int stop_inode_listening(int fd)
{
    int ret = close(fd);
    if(ret < 0)
    {
        printf("Error::start_inode_listening::inotify_add_watch :: errno %d\n",errno);
        return FERR_STOP_INODE_LISTENING;
    }
    return 0;
    
}
int read_inode(int fd,char *buf,int max_size)
{
    int bytes_read = read(fd, buf, max_size);
    if (bytes_read < 0)
    {
        printf("start_inode_listening :: read_inode :: errno %d\n",errno);
        return FERR_READ_INODE; 
    }
    return bytes_read;
}
