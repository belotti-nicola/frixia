#include "frixia_file_modify.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

int start_file_modify_listening(char *file_name)
{
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        printf("start_file_modify_listening :: inotify_init :: errno %d\n",errno);
        return FERR_START_FILE_MODIFY_INOTIFY_INIT;
    }

    int wd = inotify_add_watch(inotify_fd, file_name, IN_MODIFY);
    if (wd == -1) {
        printf("start_file_modify_listening :: inotify_add_watch :: errno %d\n",errno);
        return FERR_START_FILE_MODIFY_INOTIFY_ADD_WATCHDOG;
    }

    return inotify_fd;
}
int stop_file_modify_listening(int fd)
{
    int ret = close(fd);
    if(ret < 0)
    {
        printf("start_file_modify_listening :: inotify_add_watch :: errno %d\n",errno);
        return FERR_STOP_FILE_MODIFY_LISTENING;
    }
    
}
int read_file_modify(int fd,char *buf,int max_size)
{
    int bytes_read = read(fd, buf, max_size);
    if (bytes_read < 0)
    {
        printf("start_file_modify_listening :: read_file_modify :: errno %d\n",errno);
        return FERR_READ_FILE_MODIFY; 
    }
    return buf;
}
