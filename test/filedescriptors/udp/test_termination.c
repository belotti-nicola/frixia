#include <frixia/frixia.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <frixia/fudp_handler.h>
#include <frixia/frixia_reader.h>
#include <string.h>


#define WAIT_SECONDS 1

void *WRITER(void *arg)
{
    sleep(WAIT_SECONDS);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        perror("socket failed");
        fflush(stdout);
        return NULL;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int written_bytes = sendto(
        fd,
        "AB",
        2,
        0,
        (struct sockaddr *)&addr,
        sizeof(addr)
    );

    if (written_bytes != 2)
    {
        perror("sendto");
        return NULL;
    }

    return NULL;
}

void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    frixia_environment_t *fenv = ctx->fenv;


    int dim = frixia_get_filedescription_read_size(fenv,ctx->fd);
    char *buf = malloc(sizeof(char) * dim);
    
    frixia_read_filedescriptor(fenv,ctx->fd,buf,dim);

    free(buf);
    frixia_stop(fenv);
    return NULL;
}

int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(10);
    
    res = frixia_add_udp(fenv,"0.0.0.0",18080,1024);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding udp");
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