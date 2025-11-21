#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#include "frixia_inode.h"

int start_inode_listening(const char *path, FRIXIA_INODE_FLAG_T mask)
{
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) 
    {
        printf("inotify_init :: errno %d\n",errno);
        return FERR_START_INODE_INOTIFY_INIT;
    }

    printf("Computed mask %d\n\n",mask);
    int wd = inotify_add_watch(inotify_fd, path, mask );
    if (wd == -1) 
    {
        printf("inotify_init :: %s(mask %d, errno %d)\n",path,mask,errno);
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
