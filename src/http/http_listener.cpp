#include "http_listener.hpp"
#include <sys/socket.h>
#include <unistd.h> 
#include <stdio.h>

int HTTPListener::start(){
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == 0){
        return -1;
    }
    while(!exit_condition){
        sleep(1);
        printf("OOOO");
    }
    return 0;

}

int HTTPListener::stop(){
    exit_condition = false;
    return 0;
}