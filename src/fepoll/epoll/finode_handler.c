#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include <frixia/frixia_inode.h>
#include "finode_handler.h"

FRIXIA_INODE_FD_RESULT CREATE_FRIXIA_INODE_FD_RESULT(int fd,FINODE_CODE code, int errno_code)
{
    FRIXIA_INODE_FD_RESULT retVal = 
    {
        .fd = fd,
        .res.code = code,
        .res.errno_code = errno_code
    };

    return retVal;
}


FRIXIA_INODE_FD_RESULT start_inode_listening(const char *path, FRIXIA_INODE_FLAG mask)
{
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) 
    {
        return CREATE_FRIXIA_INODE_FD_RESULT(-1,FERR_START_INODE_INOTIFY_INIT,errno);
    }

    printf("Computed mask %d\n\n",mask);
    int wd = inotify_add_watch(inotify_fd, path, mask );
    if (wd == -1) 
    {
        printf("inotify_init :: %s(mask %d, errno %d)\n",path,mask,errno);
        return CREATE_FRIXIA_INODE_FD_RESULT(-1,FERR_START_INODE_INOTIFY_ADD_WATCHDOG,errno);
    }

    printf("start_inode_listening: fd %d, path %s\n",inotify_fd,path);
    return CREATE_FRIXIA_INODE_FD_RESULT(inotify_fd,FINODE_OK,-1);
}
FRIXIA_INODE_FD_RESULT stop_inode_listening(int fd)
{
    int ret = close(fd);
    if(ret < 0)
    {
        printf("Error::start_inode_listening::inotify_add_watch :: errno %d\n",errno);
        return CREATE_FRIXIA_INODE_FD_RESULT(-1,FERR_STOP_INODE_LISTENING,errno);
    }
    return CREATE_FRIXIA_INODE_FD_RESULT(-1,FINODE_OK,errno);
    
}
int read_inode(int fd,char buf[],int max_size)
{
    int bytes_read = read(fd, buf, max_size);
    if (bytes_read < 0)
    {
        printf("start_inode_listening :: read_inode :: errno %d\n",errno);
        return FERR_READ_INODE; 
    }

    //if you use it instead of buff it's profit
    struct inotify_event *event = (struct inotify_event *)buf;
    printf("%d %d %d %s\n",
        event->wd,
        event->mask,
        event->cookie,
        event->name
    );

    return bytes_read;
}
