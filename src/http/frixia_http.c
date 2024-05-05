#include <stdio.h>
#include <string.h>

#include "frixia_http.h"
#include "frixia_http_request.h"


enum State {
    START,
};

void parse_http_request(char *request_string, struct HTTPRequest *request){
    enum State state = START;

    printf("percs : %s\n",request_string);

}