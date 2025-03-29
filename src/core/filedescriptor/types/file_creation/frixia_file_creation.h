#ifndef FRIXIA_FILE_CREATION_H
#define FRIXIA_FILE_CREATION_H

typedef enum FFILE_CREATION
{
    FERR_START_FILE_CREATION_INOTIFY_INIT=-1,
    FERR_START_FILE_CREATION_INOTIFY_ADD_WATCHDOG=-2,
    FERR_STOP_FILE_CREATION_LISTENING=-3,
    FERR_READ_FILE_CREATION=-4

} FFILE_CREATION_T;


int start_file_creation_listening(char *directory, char *file);
int stop_file_creation_listening(int fd);
int read_file_creation(int fd,char *buf,int max_size);
#endif