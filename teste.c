
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int id[10];
int main(){
    for(int i =0; i< 10; i++){
    id[i] = 0;
    printf("%d",id[i]);
    }
}
