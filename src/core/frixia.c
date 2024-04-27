#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#include "frixia.h"


//expected fds to monitor. Just a kernel hint
//define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

//max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

#define FRIXIA_READ_SIZE 10

enum possible_returns {
    OK,
    ERR_EPOLL_CREATE,
    ERR_EPOLL_CTL,
    ERR_CHANGE_FILEDESCRIPTOR,
    ERR_MKFIFO,
    ERR_CHANGEPIPE_MKFIFO,
    ERR_CHANGEPIPE_OPENINGFD,
    ERR_EPOLL_WAIT
};


int frixia_start(){
    //create epoll
	int epoll_fd = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if ( epoll_fd < 0 ) {
        return ERR_EPOLL_CREATE;
    }
    printf("EPOLL FILE DESCRIPTOR:: %d\n",epoll_fd);

    //create the change epoll filedescriptor
    //(which is a pipe)
    const char *fname = "change_epoll_ctl";
    if (mkfifo(fname, 0666)) {
        return ERR_CHANGEPIPE_MKFIFO;
    }
    int change_fd = open(fname, O_RDONLY);
    if (change_fd == -1) {
        return ERR_CHANGEPIPE_OPENINGFD;
    }
    printf("EPOLL CHANGE::%d",change_fd);

    //add the "change-epoll_ctl" fd
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = change_fd;
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,change_fd, &ev); 
    if (epoll_ctl_retval == -1) {
        return ERR_EPOLL_CTL;
    }
    
    //start epoll
    int events_number;
	bool keep_looping = true;
    struct epoll_event *events;
    events = calloc(FRIXIA_EPOLL_WAIT_MAX_SIZE, sizeof(struct epoll_event));
    char read_buffer[FRIXIA_READ_SIZE + 1];
    while(keep_looping){
        events_number = epoll_wait(epoll_fd, events ,FRIXIA_EPOLL_WAIT_MAX_SIZE, -1);
        if(events_number == -1 ){
            return ERR_EPOLL_WAIT;
        }
        for(int i=0;i<events_number;i++){
            if(events[i].data.fd == change_fd){
                keep_looping = false;
            }
        }
    }

    return OK;
}

int frixia_stop(){
    return OK;
}

int frixia_start_http_listening(){
    return 1;
}