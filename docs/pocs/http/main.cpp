

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <iostream>

#define PORT 8080
#define MAX_CON 1
#define MAX_DATA_SIZE 4096

int main(int argc, char *argv[])
{	
	struct sockaddr_in serveraddr;
	char buffer[MAX_DATA_SIZE] = {0};

	int socket_fd;
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
		printf("ERROR CREATING SOCKET FILE DESCRIPTOR");
        return -1;
    }
	int reuse=1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse))<0){
    	printf("setsockopt(SO_REUSEADDR) failed");
		return -1;
	}


	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
	int retVal = bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(retVal < 0)
	{
		printf("bind error!!%d",errno);
		return -3;
	}

	if(listen(socket_fd, 10) == -1)
    {
    	printf("listen error!!");
		return -3;
    }

	struct epoll_event *events;
	events = (epoll_event*)calloc(MAX_CON, sizeof(struct epoll_event));
    
	int epoll_fd;
	if ((epoll_fd = epoll_create(MAX_CON)) == -1) {
        printf("listen error!!");
		return -4;
    }
	
	struct epoll_event ev;
    ev.events =  EPOLLET;
    ev.data.fd = socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev) < 0) {
        printf("listen error!!");
		return -5;
    }

	int events_number;
	for(int i=0;i<2;i++){
		events_number = epoll_wait(epoll_fd, events, MAX_CON, -1);
		printf("EVENTS_NUMBER IS %d\n",events_number);
		for(int j=0;j<events_number;j++){
			struct sockaddr in_addr;
			socklen_t in_len;
            int client_fd;

            in_len = sizeof(in_addr);
            client_fd = accept(socket_fd, &in_addr, &in_len);
			if(client_fd < 0){
				printf("accept error!!");
				return -6;
			}

			memset(buffer, 0, sizeof(buffer));
			int size = read(client_fd, buffer, sizeof(buffer));
			if ( size < 0 ) {
				printf("read error");
				return -7;
			}
			
			if (write(client_fd, buffer, size) < 0) {
				printf("write error");
				return -8;
			}
			close(client_fd);
			printf("closed\n");
		}	
	}
        
    printf("closed epoll_fd\n");
	close(epoll_fd);
	printf("closed socket_fd\n");
	close(socket_fd);
	printf("freed events alloc\n");
	free(events);

    return 0;
}