#include <frixia/frixia.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>

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
    printf("*********************\n");
    int fd = ctx->fd;

    struct sockaddr in_addr;
    socklen_t in_len;

    in_len = sizeof(in_addr);
    printf("accepting...\n");
    int reply = accept(fd, &in_addr, &in_len);
    if (reply == -1)
    {
        perror("Error in accept!\n");
    }
    char buffer[1024] = {0};
    printf("reading...\n");
    int read_bytes = read(reply,buffer,1024);
    if ( read_bytes == -1 )
    {
        printf("Error reading!\n");
    }
    
    int *counter = ctx->sv.auxiliary; 
    *counter += 1;

    frixia_stop(ctx->fenv);
    return NULL;
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
    setbuf(stdout, NULL);

    FRIXIA_RESULT res;
    int counter = 0;
    frixia_environment_t *fenv = frixia_environment_create(10);
    
    res = frixia_add_tcp(fenv,"0.0.0.0",18080,1024);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding tcp");
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,TEST_CALLBACK,&counter);

    pthread_t th;
    pthread_create(&th,NULL,WRITER,(void *)&fd);

    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    
    pthread_join(th, NULL);

    if ( counter != 1 )
    {
        printf("Error: test counter value is wrong after execution\n");
        return 1;
    }
    return 0;
}