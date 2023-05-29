#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void signalHandler(int signalReceived){
    if(signalReceived == SIGUSR1){
        printf("Signal SIGUSR1 has been captured\n");
        exit(0);
    }
}

int main(){
    int pid;
    pid = getpid();
    printf("PID = %d\n",pid);
    signal(SIGUSR1,signalHandler);
    while(1);
    return 0;
}
