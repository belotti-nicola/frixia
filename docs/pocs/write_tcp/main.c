#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(18080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    const char *msg = "Hello server\n";
    write(fd, msg, strlen(msg));

    close(fd);
}