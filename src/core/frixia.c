#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>


#include "frixia.h"
#include "../http/frixia_http.h"
#include "../http/frixia_http_request.h"



//expected fds to monitor. Just a kernel hint
//define it as positive non null
#define FRIXIA_EPOLL_KERNEL_HINT 5

//max events definition for epoll_wait
#define FRIXIA_EPOLL_WAIT_MAX_SIZE 10

//COMMAND LENGTH READING THE PIPE
#define FRIXIA_READ_SIZE 64


enum possible_returns {
    OK,
    ERR_EPOLL_CREATE,
    ERR_EPOLL_CTL,
    ERR_CHANGE_FILEDESCRIPTOR,
    ERR_MKFIFO,
    ERR_CHANGEPIPE_MKFIFO,
    ERR_CHANGEPIPE_OPENINGFD,
    ERR_EPOLL_WAIT,
    ERR_EPOLL_CTL_ADDHTTP,
    ERR_HTTP_SOCKET,
    ERR_HTTP_SETSOCKETOPT,
    ERR_HTTP_BIND,
    ERR_HTTP_LISTEN,
    ERR_HTTP_STOP,
    ERR_UDP_SOCKET,
    ERR_UDP_ADD,
    ERR_UDP_LISTEN,
    ERR_UDP_NONBLOCKING,
    ERR_UDP_STOP,
    ERR_STOPPING_FRIXIA_HTTP,
    ERR_STOPPING_FRIXIA_UDP,
    ERR_ACCEPTING_TCP,
    ERR_READING_TCP,
    ERR_WRITING_TCP

};


int frixia_start(){
    int http_fd=-1,udp_fd;

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
    printf("EPOLL CHANGE::%d\n",change_fd);

    //add the "change-epoll_ctl" fd
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
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
            // CHANGE EPOLL POLICY (ADD/DEL/MOD)
            printf("%d\n",events[i].data.fd);
            if(events[i].data.fd == change_fd){
                char buf[FRIXIA_READ_SIZE];
                read(change_fd,buf,FRIXIA_READ_SIZE+1);
                printf("::%s\n",buf);
                if(strcmp(buf, "STOP ALL\n") == 0){
                    keep_looping = false;
                    return frixia_stop(http_fd, udp_fd);
                } 
                if(strcmp(buf, "START HTTP\n") == 0) 
                {
                    printf("START HTTP %d\n",http_fd);
                    if(http_fd == -1 ){
                        http_fd = socket(AF_INET, SOCK_STREAM, 0);
                        if(http_fd == -1)
                        {
                            printf("ERROR CREATING SOCKET FILE DESCRIPTOR\n");
                            return ERR_HTTP_SOCKET;
                        }
                        printf("http %d\n",http_fd);
                        int reuse=1;
                        if (setsockopt(http_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse))<0){
                            printf("setsockopt(SO_REUSEADDR) failed\n");
                            return ERR_HTTP_SETSOCKETOPT;
                        }

                        struct sockaddr_in serveraddr;
                        serveraddr.sin_family = AF_INET;
                        serveraddr.sin_addr.s_addr = INADDR_ANY;
                        serveraddr.sin_port = htons(8080);
                        int retVal = bind(http_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
                        if(retVal < 0)
                        {
                            printf("bind error!!%d\n",errno);
                            return ERR_HTTP_BIND;
                        }

                        if(listen(http_fd, 10) == -1)
                        {
                            printf("listen error!!\n");
                            return ERR_HTTP_LISTEN;
                        }
                        struct epoll_event ev_http;
                        ev_http.events = EPOLLIN | EPOLLET;
                        ev_http.data.fd = http_fd;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, http_fd, &ev_http) < 0) {
                            printf("%d\n",errno);
                            return ERR_EPOLL_CTL_ADDHTTP;
                        }
                    }           
                }
                if(strcmp(buf, "STOP HTTP\n") == 0 ) 
                {
                    printf("STOP HTTP %d\n",http_fd);
                    if (http_fd > 0 ){
                        epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD,http_fd, NULL); 
                        if(epoll_ctl_retval == -1){
                            printf("%d\n",errno);
                            return ERR_HTTP_STOP;
                        }
                    }
                }
                if(strcmp(buf, "START UDP\n") == 0) {
                    struct sockaddr_in servaddr, cliaddr; 
                    
                    // Creating socket file descriptor
                    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
                    if ( udp_fd == -1 ) { 
                        return ERR_UDP_SOCKET; 
                    }
                                       
                    // Filling server information 
                    servaddr.sin_family    = AF_INET; // IPv4 
                    servaddr.sin_addr.s_addr = INADDR_ANY; 
                    servaddr.sin_port = htons(8080); 
                    int bind_ret_val = bind(udp_fd,(const struct sockaddr *)&servaddr,sizeof(servaddr)); 
                    if ( bind_ret_val == -1 ) { 
                        return ERR_UDP_SOCKET;  
                    }

                    struct epoll_event ev_udp;
                    ev_udp.events = EPOLLIN | EPOLLET;
                    ev_udp.data.fd = udp_fd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_fd, &ev_udp) < 0) {
                        printf("%d\n",errno);
                        return ERR_UDP_ADD;
                    }                                    
                } 
                if(strcmp(buf, "STOP UDP\n") == 0) {
                    printf("STOP UDP %d\n",udp_fd);
                    if (udp_fd > 0 ){
                        epoll_ctl_retval = epoll_ctl(epoll_fd, EPOLL_CTL_DEL,udp_fd, NULL); 
                        if(epoll_ctl_retval == -1){
                            printf("%d\n",errno);
                            return ERR_UDP_STOP;
                        }
                    }
                }
            }
            
            if(events[i].data.fd == http_fd){
                char buffer[2048] = {0};
                struct sockaddr in_addr;
                socklen_t in_len;
                int client_fd;

                in_len = sizeof(in_addr);
                client_fd = accept(http_fd, &in_addr, &in_len);
                if(client_fd == -1){
                    return ERR_ACCEPTING_TCP;
                }
                memset(buffer, 0, sizeof(buffer));
                int size = read(client_fd, buffer, sizeof(buffer));
                if ( size < 0 ) {
                    return ERR_READING_TCP;
                }
                //DO PROCESSING SOMEWAY AND COMPUTE ANSWER (WHICH IS BUFFER)
                struct HTTPRequest r;
                parse_http_request(buffer,&r);
                printf("%s\n",r.method);
                if (write(client_fd, buffer, size) < 0) {
				    return ERR_WRITING_TCP;
                }
			    close(client_fd);
            }
            
            if(events[i].data.fd == udp_fd){
                printf("UDP \n");
            }
            
                        
        }
    }

    return OK;
}

int frixia_stop(int http_fd,int udp_fd){
    if(http_fd > 2){
        FILE *fp = fopen("change_epoll_ctl", "ab");
        if (fp == NULL){
            return ERR_STOPPING_FRIXIA_HTTP;
        }
        fputs("STOP_HTTP\n", fp);
        fclose(fp);
    }
    if(udp_fd > 2){
        FILE *fp = fopen("change_epoll_ctl", "ab");
        if (fp == NULL){
            return ERR_STOPPING_FRIXIA_UDP;
        }
        fputs("STOP UPD\n", fp);
        fclose(fp);
    }

    return OK;
}

