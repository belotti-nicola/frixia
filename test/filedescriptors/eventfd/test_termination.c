#include <frixia/frixia.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <frixia/ftcp_handler.h>
#include <frixia/frixia_reader.h>
#include <string.h>
#include <errno.h>


#define WAIT_SECONDS 1

void *WRITER(void *arg)
{
    sleep(1);
    int *fd = (int *)arg;
    uint64_t val = 1;
    int written_bytes = write(*fd, &val, sizeof(val));
    if ( written_bytes <= 0)
    {
        printf("Error writing on fd %d(errno: %d,%s)\n",written_bytes,errno,strerror(errno));
    }
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

    frixia_environment_t *fenv = frixia_environment_create(10);

    FRIXIA_RESULT res;
    res = frixia_add_eventfd(fenv);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding eventfd\n");
        return -1;
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,TEST_CALLBACK,NULL);

    pthread_t th;
    pthread_create(&th,NULL,WRITER,(void *)&fd);

    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    
    pthread_join(th, NULL);
    return 0;
}