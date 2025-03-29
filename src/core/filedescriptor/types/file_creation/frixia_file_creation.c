#include "frixia_file_creation.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

int start_file_creation_listening(char *directory, char *file)
{
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        printf("inotify_init :: errno %d\n",errno);
        return FERR_START_FILE_CREATION_INOTIFY_INIT;
    }

    int wd = inotify_add_watch(inotify_fd, directory, IN_CREATE );
    if (wd == -1) {
        printf("inotify_add_watch :: errno %d\n",errno);
        return FERR_START_FILE_CREATION_INOTIFY_ADD_WATCHDOG;
    }

    printf("start_file_creation_listening: fd %d, file %s\n",inotify_fd,file);
    return inotify_fd;
}
int stop_file_creation_listening(int fd)
{
    int ret = close(fd);
    if(ret < 0)
    {
        printf("start_file_creation_listening :: inotify_add_watch :: errno %d\n",errno);
        return FERR_STOP_FILE_CREATION_LISTENING;
    }
    
}
int read_file_creation(int fd,char *buf,int max_size)
{
    int bytes_read = read(fd, buf, max_size);
    if (bytes_read < 0)
    {
        printf("start_file_creation_listening :: read_file_creation :: errno %d\n",errno);
        return FERR_READ_FILE_CREATION; 
    }
    return buf;
}
