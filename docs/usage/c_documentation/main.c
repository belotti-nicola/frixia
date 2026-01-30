#include <frixia/frixia.h>
#include <unistd.h>
#include <stdio.h>

#define MAXIMUM_FILE_DESCRIPTORS 25

//TODO THIS IS GOING TO BE DELETED
#define SECONDS 10
#include <sys/socket.h>
#include <string.h>
char HTTP_OK[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 28\r\n"
    "\r\n"
    "Greetings from frixia engine";
void *stop_in_seconds_cb(void *arg)
{
    for(int i=SECONDS;i>0;i--) { sleep(1); printf("\t-%d seconds to shutdown\n",i); }
    
    frixia_environment_t *fenv = (frixia_environment_t *)arg;
    int rc = frixia_stop(fenv);
    if ( rc != 0 )
    {
        printf("Error engine_stopper!\n");
        return NULL;
    }
    printf("OK engine_stopper!\n");
    return NULL;
}

void *engine_stop_cb(ss_worker_ctx_t *ctx)
{
    frixia_environment_t *fenv = (frixia_environment_t *)ctx->shinsu_senju_ctx->ctx;
    frixia_stop(fenv);
    int rc = frixia_stop(fenv);
    if ( rc != 0 )
    {
        printf("Error engine_stopper!\n");
        return NULL;
    }
    printf("OK engine_stopper!\n");
    return NULL;
}

int main()
{
    frixia_environment_t *fenv = frixia_environment_create(MAXIMUM_FILE_DESCRIPTORS);
    FRIXIA_RESULT frixia_waker_res = frixia_add_eventfd(fenv);
    if( !frixia_result_is_ok(frixia_waker_res) )
    {
        printf("Error adding eventfd\n");
        return -1;
    }
    
    
    FRIXIA_RESULT TCP_RES;
    TCP_RES = frixia_add_tcp(fenv,"127.0.0.1",18080,1024);
    if( !frixia_result_is_ok(TCP_RES) )
    {
        printf("Error adding TCP\n");
        return -1;
    }
    int tcp_fd = frixia_result_fd(TCP_RES);
    frixia_register_callback(fenv,tcp_fd,NULL,NULL);//TODO register_http...(1.1)

    FRIXIA_RESULT UDP_RES = frixia_add_udp(fenv,"127.0.0.1",19600,1024);
    if( !frixia_result_is_ok(UDP_RES) )
    {
        printf("Error adding udp\n");
        return -1;
    }
    int udp_res = frixia_result_fd(UDP_RES);
    frixia_register_callback(fenv,udp_res,engine_stop_cb,NULL);

    FRIXIA_RESULT SIGINT_RES = frixia_add_signal(fenv,FSIGNAL_INT);
    if( !frixia_result_is_ok(SIGINT_RES) )
    {
        printf("Error adding SIGINT\n");
        return -1;
    }
    int sigint_fd = frixia_result_fd(SIGINT_RES);
    printf("%d sigint_fd\n",sigint_fd);
    frixia_register_callback(fenv,sigint_fd,(void *)frixia_stop,fenv);


    pthread_t th;
    pthread_create(&th,NULL,stop_in_seconds_cb,fenv);


    frixia_start(fenv);
    
    frixia_environment_destroy(fenv); 
    printf("Ended correctly.\n");
    return 0;
}
