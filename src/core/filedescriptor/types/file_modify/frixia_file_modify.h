#ifndef FRIXIA_FILE_MODIFY_H
#define FRIXIA_FILE_MODIFY_H

typedef enum FFILE_MODIFY
{
    FERR_START_FILE_MODIFY_INOTIFY_INIT,
    FERR_START_FILE_MODIFY_INOTIFY_ADD_WATCHDOG,
    FERR_STOP_FILE_MODIFY_LISTENING,
    FERR_READ_FILE_MODIFY

} FFILE_MODIFY_T;


int start_file_modify_listening(char *file_name);
int stop_file_modify_listening(int fd);
int read_file_modify(int fd,char *buf,int max_size);
#endif