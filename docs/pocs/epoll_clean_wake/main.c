#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>


void *th_waker(void *arg)
{
    int sleep_time = 5;
    int sleep_duration = 1;

    printf("Thread started. Will wake epoll in %d. Countdown about to start\n",sleep_time);
    while ( sleep_time > 0 )
    {
        printf("\t-%d seconds to wake...\n",sleep_time);
        sleep(1);
        sleep_time -= 1;
    }

    printf("About to waking epoll...\n");
    int *fd = (int *)arg;
    uint64_t one = 1;
    write(*fd, &one, sizeof(one));
    
}

int main() {
    int efd = epoll_create1(0);
    int waker_fd = eventfd(0, EFD_NONBLOCK);
    
    struct epoll_event ev = { .events = EPOLLIN, .data.fd = waker_fd };
    epoll_ctl(efd, EPOLL_CTL_ADD, waker_fd, &ev);

    pthread_t waker; 
    pthread_create(&waker,NULL,th_waker,&waker_fd);
    
    struct epoll_event events[1];
    epoll_wait(efd, events, 1, -1);
    printf("Epoll waked!\n");

    pthread_join(waker,NULL);

    close(waker_fd);
    close(efd);

    printf("POC ended.\n");
    return 0;
}
