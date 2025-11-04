#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

#define BUF_SIZE 4096
#define MAX_EVENTS 4

// Imposta un file descriptor come non-bloccante
static int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(void) {
    int master_fd;
    char *slave_name;

    // 1) Crea il PTY master
    master_fd = posix_openpt(O_RDWR | O_NOCTTY);
    if (master_fd < 0) {
        perror("posix_openpt");
        return 1;
    }

    if (grantpt(master_fd) < 0) {
        perror("grantpt");
        return 1;
    }
    if (unlockpt(master_fd) < 0) {
        perror("unlockpt");
        return 1;
    }

    slave_name = ptsname(master_fd);
    if (!slave_name) {
        perror("ptsname");
        return 1;
    }

    printf("Simulatore seriale avviato.\n");
    printf("Lato slave: %s\n", slave_name);
    printf("Aprilo da un altro terminale (es: screen %s 9600)\n", slave_name);

    // 2) Imposta non-blocking
    set_nonblocking(master_fd);
    set_nonblocking(STDIN_FILENO);

    // 3) Crea epoll
    int epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        return 1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = master_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, master_fd, &ev) < 0) {
        perror("epoll_ctl master");
        return 1;
    }

    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0) {
        perror("epoll_ctl stdin");
        return 1;
    }

    printf("In ascolto. Scrivi qui per inviare al client.\n");

    char buf[BUF_SIZE];
    struct epoll_event events[MAX_EVENTS];

    // 4) Loop principale
    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if (fd == master_fd) {
                // Dati dallo slave (il “client”)
                ssize_t r = read(master_fd, buf, sizeof(buf));
                if (r > 0) {
                    write(STDOUT_FILENO, "[RX] ", 5);
                    write(STDOUT_FILENO, buf, r);
                    write(STDOUT_FILENO, "\n", 1);
                } else if (r == 0) {
                    printf("Slave scollegato\n");
                    goto cleanup;
                } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read master_fd");
                    goto cleanup;
                }

            } else if (fd == STDIN_FILENO) {
                // Input locale → invia allo slave
                ssize_t r = read(STDIN_FILENO, buf, sizeof(buf));
                if (r > 0) {
                    write(master_fd, buf, r);
                }
            }
        }
    }

cleanup:
    close(epfd);
    close(master_fd);
    return 0;
}
