#include "src/core/frixia_h.h"

int main(int argc, char *argv[])
{  
    frixia_environment_t *env = frixia_environment_create();
    frixia_add_tcp(env,"0.0.0.0",8081,512);
    frixia_add_tcp(env,"0.0.0.0",8082,1024);
    frixia_add_udp(env,"0.0.0.0",9601,256);
    frixia_add_udp(env,"0.0.0.0",9602,400);
    return 0;
}