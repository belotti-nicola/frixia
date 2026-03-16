#define _GNU_SOURCE

#include <frixia/frixia.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/fcntl.h>
#include <frixia/finode_handler.h>
#include <frixia/frixia_reader.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>


void *WRITER(void *arg)
{
    printf("creating file\n");
    int fd = open("testfile.txt", O_RDWR | O_CREAT, 0644);
    printf("Done! %d\n", fd);
    return NULL;
}

void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    frixia_environment_t *fenv = ctx->fenv;
    frixia_stop(fenv);
    return NULL;
}


int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(10);
    
    res = frixia_add_inode(fenv,"./",FINODE_CREATE);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding inode");
        return -1;
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,TEST_CALLBACK,NULL);
    frixia_start(fenv);

    pthread_t th;
    pthread_create(&th,NULL,WRITER,NULL);

    frixia_wait(fenv);
    frixia_environment_destroy(fenv);
    
    return 0;
}