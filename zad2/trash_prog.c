//
// Created by sirius on 27.03.17.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <signal.h>

void sigxcpuHandler(int arg){
    printf("SIGXCPU catched!\n");
}

int main(){
    signal(SIGXCPU, sigxcpuHandler);
    while(1);
    return 0;
}
