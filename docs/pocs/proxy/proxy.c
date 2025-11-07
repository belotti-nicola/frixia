#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAX_EVENTS 64
#define BUF_SIZE 4096


//PARAMETERS
#define PORT 8080
#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 9000


struct conn {
    int client_fd;
    int server_fd;
};

int main() {
    int listen_fd, epfd;
    struct epoll_event ev, events[MAX_EVENTS];

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(listen_fd, 10);
    printf("Listening: %s on port %d\n","0.0.0.0",PORT);

    epfd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

    FILE *logf = fopen("proxy_log.txt", "a");

    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == listen_fd) {
                // Nuovo client
                int client_fd = accept(listen_fd, NULL, NULL);

                // Connessione verso la webapp
                int server_fd = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in saddr = {
                    .sin_family = AF_INET,
                    .sin_port = htons(SERVER_PORT) // porta della webapp
                };
                inet_pton(AF_INET, SERVER_IP, &saddr.sin_addr); // IP webapp
                connect(server_fd, (struct sockaddr *)&saddr, sizeof(saddr));

                struct conn *c = malloc(sizeof(struct conn));
                c->client_fd = client_fd;
                c->server_fd = server_fd;

                ev.events = EPOLLIN | EPOLLET;
                ev.data.ptr = c;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
                epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);
            } else {
                struct conn *c = events[i].data.ptr;
                char buf[BUF_SIZE];
                int fd = events[i].data.fd;
                int r = read(fd, buf, BUF_SIZE);
                if (r <= 0) continue;

                int dest = (fd == c->client_fd) ? c->server_fd : c->client_fd;
                write(dest, buf, r);
                fwrite(buf, 1, r, logf);
                fflush(logf);
            }
        }
    }
}
