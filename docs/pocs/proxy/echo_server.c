// echo_epoll.c
// Echo server TCP multiplo basato su epoll (non-blocking).
// Compila: gcc -O2 -Wall -o echo_epoll echo_epoll.c
// Esegui:   ./echo_epoll [porta]
// Test:     nc 127.0.0.1 9000

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_PORT 9000
#define MAX_EVENTS  64
#define READ_BUF    4096

struct client {
    int fd;
    // buffer per pending data to send (when send() non-blocking writes partial)
    char *out;
    size_t out_len;
    size_t out_off;
};

static int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) return -1;
    return 0;
}

static void close_client(int epfd, struct client *c) {
    if (!c) return;
    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
    close(c->fd);
    free(c->out);
    free(c);
}

int main(int argc, char **argv) {
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    if (port <= 0) port = DEFAULT_PORT;

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); close(listen_fd); return 1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
        perror("listen"); close(listen_fd); return 1;
    }

    if (set_nonblocking(listen_fd) < 0) {
        perror("set_nonblocking"); close(listen_fd); return 1;
    }

    int epfd = epoll_create1(0);
    if (epfd < 0) { perror("epoll_create1"); close(listen_fd); return 1; }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = NULL; // listener identified by NULL
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
        perror("epoll_ctl ADD listen"); close(listen_fd); close(epfd); return 1;
    }

    printf("Echo server listening on 0.0.0.0:%d\n", port);

    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.ptr == NULL) {
                // event sul listener -> accetta nuove connessioni
                while (1) {
                    struct sockaddr_in cli_addr;
                    socklen_t cli_len = sizeof(cli_addr);
                    int cfd = accept(listen_fd, (struct sockaddr*)&cli_addr, &cli_len);
                    if (cfd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        perror("accept");
                        break;
                    }

                    // disabilita Nagle per reattività (opzionale)
                    int one = 1;
                    setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

                    if (set_nonblocking(cfd) < 0) {
                        perror("set_nonblocking client");
                        close(cfd);
                        continue;
                    }

                    struct client *cl = calloc(1, sizeof(struct client));
                    if (!cl) { close(cfd); continue; }
                    cl->fd = cfd;
                    cl->out = NULL;
                    cl->out_len = cl->out_off = 0;

                    struct epoll_event cev;
                    cev.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR;
                    cev.data.ptr = cl;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cev) < 0) {
                        perror("epoll_ctl ADD client");
                        close(cfd); free(cl);
                        continue;
                    }

                    char ipbuf[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &cli_addr.sin_addr, ipbuf, sizeof(ipbuf));
                    printf("Accepted %s:%d -> fd %d\n", ipbuf, ntohs(cli_addr.sin_port), cfd);
                }
            } else {
                struct client *cl = (struct client*)events[i].data.ptr;

                // errori/hangup
                if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                    printf("Client fd %d closed (hup/err)\n", cl->fd);
                    close_client(epfd, cl);
                    continue;
                }

                // dati da leggere
                if (events[i].events & EPOLLIN) {
                    while (1) {
                        char buf[READ_BUF];
                        ssize_t r = recv(cl->fd, buf, sizeof(buf), 0);
                        if (r < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                            perror("recv");
                            close_client(epfd, cl);
                            break;
                        } else if (r == 0) {
                            // client chiuso volontariamente
                            printf("Client fd %d closed connection\n", cl->fd);
                            close_client(epfd, cl);
                            break;
                        } else {
                            // echo: prova a inviare subito
                            ssize_t sent = send(cl->fd, buf, r, 0);
                            if (sent < 0) {
                                if (errno == EAGAIN || errno == EWOULDBLOCK) sent = 0;
                                else {
                                    perror("send");
                                    close_client(epfd, cl);
                                    break;
                                }
                            }

                            if ((size_t)sent < (size_t)r) {
                                // dati non inviati completamente -> appendi al buffer out
                                size_t remaining = (size_t)r - (size_t)sent;
                                char *newbuf = realloc(cl->out, cl->out_len + remaining);
                                if (!newbuf) {
                                    perror("realloc");
                                    close_client(epfd, cl);
                                    break;
                                }
                                cl->out = newbuf;
                                memcpy(cl->out + cl->out_len, buf + sent, remaining);
                                cl->out_len += remaining;

                                // abilita EPOLLOUT per svuotare il buffer
                                struct epoll_event mod;
                                mod.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR;
                                mod.data.ptr = cl;
                                if (epoll_ctl(epfd, EPOLL_CTL_MOD, cl->fd, &mod) < 0) {
                                    perror("epoll_ctl MOD add EPOLLOUT");
                                    close_client(epfd, cl);
                                }
                                // non continuare a read immediatamente se buffer pieno,
                                // ma possiamo comunque continuare la receive loop
                            }

                            // per debug:
                            // stampo i dati ricevuti (troncati se binari)
                            printf("fd %d recv %zd bytes\n", cl->fd, r);
                        }
                    } // end while recv
                }

                // ready to write pending data
                if (events[i].events & EPOLLOUT) {
                    while (cl->out_len > cl->out_off) {
                        ssize_t w = send(cl->fd, cl->out + cl->out_off, cl->out_len - cl->out_off, 0);
                        if (w < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                            perror("send (EPOLLOUT)");
                            close_client(epfd, cl);
                            break;
                        }
                        cl->out_off += (size_t)w;
                    }

                    if (cl->out_off == cl->out_len) {
                        // tutto inviato -> libera buffer e togli EPOLLOUT
                        free(cl->out);
                        cl->out = NULL;
                        cl->out_len = cl->out_off = 0;

                        struct epoll_event mod;
                        mod.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR;
                        mod.data.ptr = cl;
                        if (epoll_ctl(epfd, EPOLL_CTL_MOD, cl->fd, &mod) < 0) {
                            perror("epoll_ctl MOD remove EPOLLOUT");
                            close_client(epfd, cl);
                        }
                    } else {
                        // compattiamo il buffer per non crescere indefinitamente
                        if (cl->out_off > 0) {
                            size_t remaining = cl->out_len - cl->out_off;
                            memmove(cl->out, cl->out + cl->out_off, remaining);
                            cl->out_len = remaining;
                            cl->out_off = 0;
                            char *shrunk = realloc(cl->out, cl->out_len);
                            if (shrunk) cl->out = shrunk;
                        }
                    }
                }
            } // end else client event
        } // end for events
    } // end while

    close(listen_fd);
    close(epfd);
    return 0;
}
