#include <stdio.h>
#include <string.h>

#define MAX_METHOD_LENGTH 10
#define MAX_PATH_LENGTH 100
#define MAX_HEADERS 10
#define MAX_HEADER_LENGTH 100

#define str_cmp_3(m,c0,c1,c2) \
        m[0]==c0 && m[1] ==c1 && m[2]=c2
#define str_cmp_4(m,c0,c1,c2,c4) \
        m[0]==c0 && m[1] ==c1 && m[2]=c2 && m[3]==c4


enum Method {
    PARSE_ERROR,
    GET,
    POST,
    PUT,
    PATCH,
    DELETE
};

struct HTTPRequest {
    enum Method method;
    char path[MAX_PATH_LENGTH];
    char version[100];
    char headers[MAX_HEADERS][2][MAX_HEADER_LENGTH];
    int header_count;
};

void parse_http_request(char *request_string, struct HTTPRequest *request) {
    if(*request_string == '\0'){
        printf("A");
        return;
    }

    int state = 1;
    char* ptr = request_string;
    while(*ptr != '\0'){
        switch(state){
            case -1: {
                ptr++;
                request->method = PARSE_ERROR;
                break;
            }
            case 1:{
                if(*ptr != 'G' && *ptr != 'P' && *ptr != 'D'){
                    state=-1;
                }
                if(*ptr == 'G'){
                    state=2;
                }
                if(*ptr == 'P'){
                    state=5;
                }
                if(*ptr == 'D'){
                    state=12;
                }
                ptr++;
                break;
            }
            case 2:{
                if(*ptr != 'E'){
                    state=-1;
                }
                if(*ptr == 'E'){
                    state=3;
                }
                ptr++;
                break;
            }
            case 3:{
                if(*ptr != 'T'){
                    state=-1;
                }
                if(*ptr == 'T'){
                    state=4;
                    request->method = GET;
                }
                ptr++;
                break;
            }
            case 4:{
                ptr++;
                break;
            }
            case 5:{
                if(*ptr != 'O' && *ptr != 'U' && *ptr != 'A'){
                    state=-1;
                }
                if(*ptr == 'O'){
                    state=6;
                }
                if(*ptr == 'U'){
                    state=8;
                }
                if(*ptr == 'A'){
                    state=9;
                }
                ptr++;
                break;
            }
            case 6:{
                if(*ptr != 'S'){
                    state=-1;
                }
                if(*ptr == 'S'){
                    state=7;
                }
                ptr++;
                break;
            }
            case 7:{
                if(*ptr != 'T'){
                    state=-1;
                }
                if(*ptr == 'T'){
                    state=4;
                    request->method = POST;
                }
                ptr++;
                break;
            }
            case 8:{
                if(*ptr != 'T'){
                    state=-1;
                }
                if(*ptr == 'T'){
                    state=4;
                    request->method = PUT;
                }
                ptr++;
                break;
            }
            case 9:{
                if(*ptr != 'T'){
                    state=-1;
                }
                if(*ptr == 'T'){
                    state=10;
                }
                ptr++;
                break;
            }
            case 10:{
                if(*ptr != 'C'){
                    state=-1;
                }
                if(*ptr == 'C'){
                    state=11;
                }
                ptr++;
                break;
            }
            case 11:{
                if(*ptr != 'H'){
                    state=-1;
                }
                if(*ptr == 'H'){
                    request->method = PATCH;
                    state=4;
                }
                ptr++;
                break;
            }
            case 12:{
                if(*ptr != 'E'){
                    state=-1;
                }
                if(*ptr == 'E'){
                    state=13;
                }
                ptr++;
                break;
            }
            case 13:{
                if(*ptr != 'L'){
                    state=-1;
                }
                if(*ptr == 'L'){
                    state=14;
                }
                ptr++;
                break;
            }
            case 14:{
                if(*ptr != 'E'){
                    state=-1;
                }
                if(*ptr == 'E'){
                    state=15;
                }
                ptr++;
                break;
            }
            case 15:{
                if(*ptr != 'T'){
                    state=-1;
                }
                if(*ptr == 'T'){
                    state=16;
                }
                ptr++;
                break;
            }
            case 16:{
                if(*ptr != 'E'){
                    state=-1;
                }
                if(*ptr == 'E'){
                    state=4;
                    request->method = DELETE;
                }
                ptr++;
                break;
            }
        }
    };


    printf("\nstate::%d\n",request->method);


}


int main() {
    struct HTTPRequest request;
    FILE *fp = fopen("samples/sample1.txt", "r");
    char request_string[200] = {0};
    fgets(request_string,201,fp); 

    parse_http_request(request_string, &request);

    return 0;
}