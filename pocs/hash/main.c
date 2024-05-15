#include <stdio.h>

unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int main(){
    for(unsigned int i=0;i<100;i++){
        printf("%d -> %d %d\n",i,hash(i),hash(i)%10);
    }
}