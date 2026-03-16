#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <frixia/frixia.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


#define TEST_STRING "TEST"
#define FIFO "test_fifo"

void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    frixia_environment_t *fenv = ctx->fenv;
    frixia_stop(fenv);
    return NULL;
}


int main() {
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    
    frixia_environment_t *env = frixia_environment_create(10);
    FRIXIA_RESULT res = frixia_add_fifo(env,FIFO,500);
    if( !frixia_result_is_ok(res))
    {
        printf("Error adding fifo %d(%s)\n",res.errno_code,strerror(res.errno_code));
        return 1;
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(env,fd,TEST_CALLBACK,NULL);

    frixia_start(env);

    pid_t pid = fork();
    if (pid == 0) 
    {
        int fd = open(FIFO, O_WRONLY);
        int written = write(fd, TEST_STRING, strlen(TEST_STRING));
        if (written < 0)
        {
            printf("Error writing on fifo!\n");
            return 1;
        }
        return 0;
    }

    frixia_wait(env);
    frixia_environment_destroy(env);
    unlink(FIFO);

    return 0;
}


    
