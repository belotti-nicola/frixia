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

    // int ret_code = mkfifo(FIFO, 0600);
    // if (ret_code != 0)
    // {
    //     printf("Error adding test fifo %d(%s)\n",errno,strerror(errno));
    //     return -1;
    // }

    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        int fd = open(FIFO, O_WRONLY);
        write(fd, TEST_STRING, strlen(TEST_STRING));
        return 0;
    }

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
    frixia_environment_destroy(env);
    unlink(FIFO);

    return 0;
}


    
