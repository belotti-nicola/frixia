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


#define WAIT_SECONDS 1

void *WRITER(void *arg)
{
    sleep(WAIT_SECONDS);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("connect failed");
        fflush(stdout);
        return NULL;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("connect failed");
        fflush(stdout);
        return NULL;
    }

    int written_bytes = write(fd,"AB",2);
    if ( written_bytes != 2 )
    {
        perror("write");
        return NULL;
    }

    return NULL;
}

void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    frixia_environment_t *fenv = ctx->fenv;
    frixia_stop(fenv);
    return NULL;
}


void *FDCALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    int reply = -1;
    FRIXIA_TCP_RESULT res = accept_tcp(ctx->fd,&reply);
    if ( res.exit_code != FTCP_OK )
    {
        perror("Accept failed");
    }

    convoy_t *convoy = ctx->fenv->convoy;
    const char *ip = convoy->filedescriptors[ctx->fd].type_data->tcp_info.ip;
    int port = convoy->filedescriptors[ctx->fd].type_data->tcp_info.port;
    int bytes = convoy->filedescriptors[ctx->fd].type_data->tcp_info.read_size;
    convoy_add_tcp_filedescriptor(convoy,reply,ip,port,bytes);

    void *arg = ctx->sv.auxiliary;
    frixia_register_cb(ctx->fenv,reply,TEST_CALLBACK,NULL);

    frixia_register_fepoll_events(ctx->fenv,reply);

    return NULL;
}

int main()
{
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(10);
    
    res = frixia_add_tcp(fenv,"0.0.0.0",18080,1024);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding tcp");
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,FDCALLBACK,NULL);

    pthread_t th;
    pthread_create(&th,NULL,WRITER,(void *)&fd);

    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    
    pthread_join(th, NULL);
    return 0;
}