#include <frixia/frixia.h>
#include <unistd.h>
#include <stdio.h>

#define MAXIMUM_FILE_DESCRIPTORS 25

//TODO THIS IS GOING TO BE DELETED
#include <errno.h>
#include <frixia/frixia_signal.h>
#include <signal.h>
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

void *engine_stop_cb_sigint(ss_worker_ctx_t *ctx)
{    
    
    frixia_environment_t *fenv = (frixia_environment_t *)ctx->shinsu_senju_ctx->fenv;
    int fd = ctx->id;

    struct signalfd_siginfo si;
    ssize_t r = read(fd, &si, sizeof(si));
    if (r != sizeof(si))
    {
        return NULL;
    }


    int rc = frixia_stop(fenv);
    if ( rc != 0 )
    {
        printf("Error engine_stopper!\n");
        return NULL;
    }
    printf("OK engine_stopper!\n");
    return NULL;
}

void *engine_stop_cb_udp(ss_worker_ctx_t *ctx)
{    
    int fd = ctx->id;
    //TODO THIS SUCKS   
    int dim = ctx->shinsu_senju_ctx->fenv->convoy->filedescriptors[fd].type_data->udp_info.read_size;
    char buffer[dim];
    int read_bytes = read(fd,buffer,dim);
    if (read_bytes < 0)
    {
        printf("Error engine_stopper::read! on fd %d(errno %d)\n",fd,errno);
        return NULL;
    }

    int cmp = strncmp(buffer,"STOP",4);
    if (cmp == 0)
    {
        frixia_environment_t *fenv = (frixia_environment_t *)ctx->shinsu_senju_ctx->fenv;
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

    printf("%s\n",buffer);
    return NULL;
}

int main()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    int ret = pthread_sigmask(SIG_BLOCK, &mask, NULL);
    if (ret != 0) 
    {
        printf("Error blocking sigint\n");
        return -1;
    }
    
    printf("+++++++++++++++++++++++++++++++++++++++");
    printf("\n");
    printf("pid %ld ppid %ld", (long)getpid(), (long)getppid());
    printf("\n");
    printf("+++++++++++++++++++++++++++++++++++++++");
    printf("\n");

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
    frixia_register_callback(fenv,udp_res,engine_stop_cb_udp,NULL);

    FRIXIA_RESULT SIGINT_RES = frixia_add_signal(fenv,FSIGNAL_INT);
    if( !frixia_result_is_ok(SIGINT_RES) )
    {
        printf("Error adding SIGINT\n");
        return -1;
    }
    int sigint_fd = frixia_result_fd(SIGINT_RES);
    printf("%d sigint_fd\n",sigint_fd);
    frixia_register_callback(fenv,sigint_fd,engine_stop_cb_sigint,fenv);


    // pthread_t th;
    // pthread_create(&th,NULL,stop_in_seconds_cb,fenv);

    
    frixia_start(fenv);
    printf("started ended correctly.\n");

    frixia_environment_destroy(fenv); 

    printf("Ended correctly.\n");
    return 0;
}
