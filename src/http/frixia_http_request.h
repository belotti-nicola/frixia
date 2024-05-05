#ifndef FRIXIA_HTTP_REQUEST_H
#define FRIXIA_HTTP_REQUEST_H

#define MAX_METHOD_LENGTH 10
#define MAX_PATH_LENGTH 100
#define MAX_HEADERS 10
#define MAX_HEADER_LENGTH 100

struct HTTPRequest {
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char headers[MAX_HEADERS][2][MAX_HEADER_LENGTH];
    int header_count;
};

#endif
