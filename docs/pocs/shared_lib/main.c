#include "callback_lib.h"
#include "stdio.h"

// Example callback function
void foo(char *path, char *headers, char **header_array, int header_size, void *anything) {
    printf("Path: %s\n", path);
    printf("Headers: %s\n", headers);
    for (int i = 0; i < header_size; i++) {
        printf("Header %d: %s\n", i, header_array[i]);
    }
    printf("Anything: %s\n", (char *)anything);
}

int main() {
    // Register the user-defined callback
    register_callback(foo);

    // Example data
    char *path = "/api/v1/test";
    char *headers = "Content-Type: application/json";
    char *header_array[] = {"Host: example.com", "Connection: keep-alive"};
    int header_size = 2;
    char *anything = "Extra Data";

    // Execute the callback dynamically
    execute_callback(path, headers, header_array, header_size, (void *)anything);

    return 0;
}
