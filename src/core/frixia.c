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
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 1

#define FRIXIA_READ_SIZE 10

enum possible_returns {
    OK,
    ERR_EPOLL_CREATE,
    ERR_EPOLL_CTL,
    ERR_CHANGE_FILEDESCRIPTOR
};


int frixia_start(){
    //create epoll
	int epoll_fd;
	int epoll_create_retval = epoll_create(FRIXIA_EPOLL_KERNEL_HINT);
    if ( epoll_create_retval < 0 ) {
        return ERR_EPOLL_CREATE;
    }
    printf("EPOLL FILE DESCRIPTOR %d\n",epoll_create_retval);

    //create the change epoll filedescriptor
    int change_epoll_fd = open("/home/nicola/Projects/FRIXIA/frixia/build/aaa",
                               O_RDONLY,
                               0777);
    if(change_epoll_fd == -1){
        return ERR_CHANGE_FILEDESCRIPTOR;
    }
    printf("EPOLL FILE DESCRIPTOR %d\n",change_epoll_fd);

    //add the "change-epoll_ctl" fd
    struct epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.fd = epoll_fd;
    int epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, change_epoll_fd, &ev); 
    if (epoll_ctl_retval == -1) {
        printf("errno %d\n",errno);
        return ERR_EPOLL_CTL;
    }
    
    //start epoll
    int events_number;
	bool keep_looping = true;
    struct epoll_event *events;
    events = calloc(FRIXIA_EPOLL_WAIT_MAX_SIZE, sizeof(struct epoll_event));
    char read_buffer[FRIXIA_READ_SIZE + 1];
    while(keep_looping){
        events_number = epoll_wait(epoll_fd, events ,FRIXIA_EPOLL_WAIT_MAX_SIZE, 5);
        for (int i = 0; i < events_number; ++i) {
            int bytes_read = read(events[i].data.fd, read_buffer, FRIXIA_READ_SIZE);
			printf("%zd bytes read.\n", bytes_read);
			read_buffer[bytes_read] = '\0';
			printf("Read '%s'\n", read_buffer);
		
			if(!strncmp(read_buffer, "stop\n", 5))
            keep_looping = false;
        }
        printf("DDD\n");
    }

    return OK;
}

int frixia_stop(){
    return OK;
}

int frixia_start_http_listening(){
    return 1;
}