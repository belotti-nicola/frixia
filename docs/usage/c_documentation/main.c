#include <frixia/frixia.h>
#include <unistd.h>
#include <stdio.h>

#define MAXIMUM_FILE_DESCRIPTORS 25

//TODO THIS IS GOING TO BE DELETED
#include <sys/socket.h>
#include <string.h>
char HTTP_OK[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 28\r\n"
    "\r\n"
    "Greetings from frixia engine";

int main()
{
    frixia_environment_t *fenv = frixia_environment_create(MAXIMUM_FILE_DESCRIPTORS);
    
    FRIXIA_RESULT TCP_RES;
    TCP_RES = frixia_add_tcp(fenv,"127.0.0.1",18080,1024);
    if( !frixia_result_is_ok(TCP_RES) )
    {
        printf("Error adding TCP\n");
        return -1;
    }
    int tcp_fd = frixia_result_fd(TCP_RES);
    frixia_register_callback(fenv,tcp_fd,NULL,NULL);

    FRIXIA_RESULT SIGINT_RES = frixia_add_signal(fenv,FSIGNAL_INT);
    if( !frixia_result_is_ok(SIGINT_RES) )
    {
        printf("Error adding SIGINT\n");
        return -1;
    }
    int sigint_fd = frixia_result_fd(SIGINT_RES);
    printf("%d sigint_fd\n",sigint_fd);
    frixia_register_callback(fenv,sigint_fd,(void *)frixia_stop,fenv);

    frixia_start(fenv);
    
    frixia_environment_destroy(fenv); 
    printf("Ended correctly.\n");
    return 0;
}
