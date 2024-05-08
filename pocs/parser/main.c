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
    GET,
    POST
};


enum State {
    START,
    METHOD,
    PATH,
    VERSION,
    HEADER_KEY,
    HEADER_VALUE,
    END
};

struct HTTPRequest {
    enum Method method;
    char path[MAX_PATH_LENGTH];
    char version[100];
    char headers[MAX_HEADERS][2][MAX_HEADER_LENGTH];
    int header_count;
};

void parse_http_request(char *request_string, struct HTTPRequest *request) {
    enum State state = START;
    char* aux = request_string;
    int i;
    switch(*request_string){
        case 'G':
            request->method = GET;
            aux = aux+4;
            break;
        case 'P':
            request->method = POST;
            aux = aux+5;
            break;
        default:
            break;
    }
    for(i=0;*aux != ' ';aux++,i++){
        request->path[i]=*aux;
    }
    request->path[i] = '\0';

    aux++;

    for(i=0;*aux != '\n';aux++,i++){
        request->version[i]=*aux;
    }
    request->version[i] = '\0';
    aux++;

    printf("%s-",*aux);
    for(i=0;*(aux+i)!='\0';i++){
        printf("%d-",*aux);
    }

}


int main() {
    struct HTTPRequest request;
    FILE *fp = fopen("samples/sample3.txt", "r");
    char request_string[200] = {0};
    fgets(request_string,201,fp); 

    parse_http_request(request_string, &request);
    printf("\n");
    printf("Method: %d\n", request.method);
    printf("Path: %s\n", request.path);
    printf("Version: %s\n",request.version);

    return 0;
}