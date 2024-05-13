#include <stdio.h>
#include <string.h>

#include "frixia_tcp.h"
#include "frixia_http_request.h"


enum State {
    START,
    END,
    METHOD
};

void parse_http_request(char *request_string, struct HTTPRequest *request){
    enum State state = START;

    char *token = strtok(request_string, " \n\r");
    while (token != NULL && state != END) {
        switch (state) {
            case START:
                strcpy(request->method, token);
                state = METHOD;
                break;
            case METHOD:
                strcpy(request->path, token);
                state = END;
                break;
        }
    }
}