#include <stdio.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "frixia.h"


//kernel expected fd to monitor. Just a hint
#define KERNEL_HINT 5

enum possible_returns {
    OK,
    ERR_EPOLL_CREATE
};


int frixia_start(){
    //create epoll
	int epoll_fd;
	if ( epoll_fd = epoll_create(KERNEL_HINT) < 0 ) {
        return ERR_EPOLL_CREATE;
    }

    //add the "change-epoll_ctl" fd


    //start epoll

    return OK;
}

int frixia_stop(){
    return OK;
}

int frixia_start_http_listening(){
    return 1;
}