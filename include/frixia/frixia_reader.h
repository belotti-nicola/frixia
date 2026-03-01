#ifndef FRIXIA_READER_H
#define FRIXIA_READER_H

//forward declaration
typedef struct frixia_environment frixia_environment_t;

int frixia_get_filedescription_read_size(frixia_environment_t *fenv,int fd);
int frixia_read_filedescriptor(frixia_environment_t *fenv,int fd,char *buff, int size);

#endif