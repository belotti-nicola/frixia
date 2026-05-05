#include <frixia/frixia.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <frixia/ftcp_handler.h>
#include <frixia/frixia_reader.h>
#include <string.h>
#include <frixia/protocols/http/http_utility.h>



#include <sys/inotify.h>
#include <errno.h>


void *STOPPER(FRIXIA_CALLBACK_CTX *ctx)
{
    int fd = ctx->fd;

    char buf[64] = {0};
    int bytes_read = read(fd, buf, 64);
    if (bytes_read < 0)
    {
        printf("start_inode_listening :: read_inode %d :: errno %d\n",fd,errno);
        return NULL; 
    }

       
    frixia_environment_t *fenv = ctx->fenv;
    frixia_stop(fenv);
    return NULL;
}

void *CONCRETE_HANDLER(FRIXIA_CALLBACK_CTX *ctx)
{
    int fd = ctx->fd;

    FILE *file = fopen("../site.html", "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *html = malloc(size + 1);
    fread(html, 1, size, file);
    html[size] = '\0';
    fclose(file);


    char request[2048];
    memset(request,0,2048);
    ssize_t dim = read(fd,request,2048);
    if ( dim == 0 )
    {
       return NULL; 
    }
    request[dim] = '\0';


    char header[512];
    memset(header,0,512);
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        size
    );
    write(fd, header, header_len);
    write(fd, html, size);

    
    free(html);
    return NULL;
}

void *FDCALLBACK(FRIXIA_CALLBACK_CTX *ctx)
{
    int reply = -1;
    FRIXIA_TCP_RESULT res = accept_tcp(ctx->fd,&reply);
    if ( res.exit_code != FTCP_OK )
    {
        perror("Accept failed");
    }

    convoy_t *convoy = ctx->fenv->convoy;
    const char *ip = convoy->filedescriptors[ctx->fd].type_data->tcp_info.ip;
    int port = convoy->filedescriptors[ctx->fd].type_data->tcp_info.port;
    int bytes = convoy->filedescriptors[ctx->fd].type_data->tcp_info.read_size;
    convoy_add_tcp_filedescriptor(convoy,reply,ip,port,bytes);

    void *arg = ctx->sv.auxiliary;
    frixia_register_cb(ctx->fenv,reply,CONCRETE_HANDLER,arg);
    
    frixia_register_fepoll_events(ctx->fenv,reply);

    return NULL;
}

int main()
{
    FRIXIA_RESULT res;
    frixia_environment_t *fenv = frixia_environment_create(100);
    
    res = frixia_add_tcp(fenv,"0.0.0.0",18080,1024);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding tcp");
    }
    int tcp_fd = frixia_result_fd(res);
    frixia_register_cb(fenv,tcp_fd,FDCALLBACK,NULL);
    
    res = frixia_add_inode(fenv, ".", FINODE_CREATE);
    if( !frixia_result_is_ok(res) )
    {
        perror("Error adding tcp");
    }
    int inode_fd = frixia_result_fd(res);
    frixia_register_cb(fenv,inode_fd,STOPPER,NULL);

    
    frixia_start(fenv);


    frixia_wait(fenv);
    frixia_environment_destroy(fenv);   

    printf("End.\n");
    return 0;
}