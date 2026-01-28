#include <frixia/frixia.h>
#include <unistd.h>
#include <stdio.h>


//TODO
//THIS IS GOING TO BE DELETED
#include <sys/socket.h>
#include <string.h>

#define MAXIMUM_FILE_DESCRIPTORS 25
#define SECOND_FOR_STOP          10


char HTTP_OK[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 28\r\n"
    "\r\n"
    "Greetings from frixia engine";


void *engine_stopper(void *arg)
{
    sleep(SECOND_FOR_STOP);

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

int main()
{
    frixia_environment_t *fenv = frixia_environment_create(MAXIMUM_FILE_DESCRIPTORS);
    
    FRIXIA_RESULT res;
    res = frixia_add_eventfd(fenv);
    if( !frixia_result_is_ok(res) )
    {
        printf("Error adding eventfd\n");
        return -1;
    }

    res = frixia_add_tcp(fenv,"127.0.0.1",18080,1024);
    if( !frixia_result_is_ok(res) )
    {
        printf("Error adding tcp\n");
        return -1;
    }

    int tcp_fd = frixia_result_fd(res);
    frixia_register_callback(fenv,tcp_fd,NULL,NULL);
    
    pthread_t th;
    pthread_create(&th,NULL,engine_stopper,fenv);

    frixia_start(fenv);
    
    frixia_environment_destroy(fenv);   
    return 0;
}
