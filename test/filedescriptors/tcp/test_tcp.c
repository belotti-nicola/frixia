#include <frixia/frixia.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <unistd.h>

#define WAIT_SECONDS 1

void WRITER(void *arg)
{
    int *fd = (int *)arg;
    printf("\n+++++++%d fd++++++\n\n",*fd);

    sleep(WAIT_SECONDS);
    int written_bytes = write(*fd,"A",1);
    if ( written_bytes != 1 )
    {
        printf("Error %d!\n",__LINE__);
        return;
    }
    printf("end\n");
    return;
}

void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    printf("\n+++++++++++++\n\n");
    char buffer[1024] = {0};
    
    //TODO: THIS GOES IN API CALL
    int read_bytes = read(4,buffer,1024);
    
    int *counter = ctx->sv.auxiliary; 
    *counter += 1;

    frixia_stop(ctx->fenv);
    return NULL;
}

int main()
{
    FRIXIA_RESULT res;
    int counter = 0;
    frixia_environment_t *fenv = frixia_environment_create(10);

    res = frixia_add_tcp(fenv,"0.0.0.0",18080,1024);
    if( !frixia_result_is_ok(res) )
    {
        printf("Error adding tcp\n");
        return -1;
    }
    int fd = frixia_result_fd(res);
    if( fd != 4 )
    {
        printf("Error tcp fd value != 4\n");
        return -1;
    }
    frixia_register_cb(fenv,fd,TEST_CALLBACK,&counter);

    printf("\n+++++++++++++\n\n");
    pthread_t th;
    pthread_create(&th,NULL,WRITER,(void *)fd);

    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    return 0;
}