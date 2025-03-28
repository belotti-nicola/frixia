#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_to_watch>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(inotify_fd, argv[1], IN_CREATE);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = inotify_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    printf("Monitoring directory: %s\n", argv[1]);

    while (1) {
        struct epoll_event events[10];
        int num_events = epoll_wait(epoll_fd, events, 10, -1);
        if (num_events == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == inotify_fd) {
                char buffer[BUF_LEN];
                int length = read(inotify_fd, buffer, BUF_LEN);
                if (length < 0) {
                    perror("read");
                }
                
                int offset = 0;
                while (offset < length) {
                    struct inotify_event *event = (struct inotify_event *) &buffer[offset];
                    if (event->mask & IN_CREATE) {
                        printf("File created: %s\n", event->name);
                    }
                    offset += EVENT_SIZE + event->len;
                }
            }
        }
    }

    close(inotify_fd);
    close(epoll_fd);
    return 0;
}
