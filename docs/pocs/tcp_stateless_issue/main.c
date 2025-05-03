#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9601

int main() {
    int server_fd, reply_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Bind the socket to the specified port
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

    // 3. Start listening for incoming connections
    listen(server_fd, 1);
    printf("Waiting for connections...\n");

    // 4. Accept a single incoming connection (persistent connection)
    reply_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
    printf("Connected to HMI client\n");

    // 5. Loop to receive FINS commands
    while (1) {
        uint8_t buffer[1024];
        ssize_t n = read(reply_fd, buffer, sizeof(buffer));

        if (n <= 0) {
            printf("Connection closed or error\n");
            break;
        }

        printf("Received %zd bytes:\n", n);
        for (int i = 0; i < n; ++i) {
            printf("%02X ", buffer[i]);
        }
        printf("\n");

        if (n <= 15 )
        {
            printf("Error reading bytes (read %ld instead of 15)\n",n);
            break;
        }

        if ( buffer[8] != 0x00 )
        {
            printf("Error buffer[8] is wrong!\n");
            break;
        }

        if ( buffer[9] != 0x00 )
        {
            printf("Error buffer[9] is wrong!\n");
            break;
        }

        if ( buffer[10] != 0x00 )
        {
            printf("Error buffer[10] is wrong!\n");
            break;
        }
        if ( buffer[11] != 0x00 )
        {
            printf("Error buffer[11] is wrong!\n");
            break;
        }

        uint8_t fins_response[] = {
            'F','I','N','S',             // Header
            0x00, 0x00, 0x00, 0x0C,      // Length = 20 bytes
            0x00, 0x00, 0x00, 0x01,      // Command = response frame
            0x00, 0x00, 0x00, 0x00,      // No error
            0x00, 0x00, 0x00, 0x00,      // Fake data (e.g., read response)
            0x00, 0x00, 0x00, 0x00
        };
        n = write(reply_fd, fins_response, sizeof(fins_response));
        if ( n < 0 )
        {
            printf("Error writing!\n");
            break;
        }
        printf("Wrote successfully %ld bytes\n",n);

    }

    // Close the connection
    close(reply_fd);
    close(server_fd);
    return 0;
}
