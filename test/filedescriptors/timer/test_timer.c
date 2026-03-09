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


void *TEST_CALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    frixia_environment_t *fenv = ctx->fenv;
    int *counter =  ctx->sv.auxiliary;

    int dim = frixia_get_filedescription_read_size(fenv,ctx->fd);
    char *buf = malloc(sizeof(char) * dim);
    frixia_read_filedescriptor(fenv,ctx->fd,buf,dim);
    free(buf);
    
    if(*counter < 5 )
    {
        *counter += 1;
        return NULL;
    }

    detached_shinsu_senju_stop_worker(ctx->fenv->shinsu_senju_ctx,ctx->fd);
    frixia_stop(fenv);
    return NULL;
}

int main()
{
    int test_counter = 0;
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);

    frixia_environment_t *fenv = frixia_environment_create(10);
    
    FRIXIA_RESULT res = frixia_add_timer(fenv,0,200,0,100);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding udp");
    }
    int fd = frixia_result_fd(res);
    frixia_register_cb(fenv,fd,TEST_CALLBACK,&test_counter);


    frixia_start(fenv);
    frixia_environment_destroy(fenv);
    
    return 0;
}