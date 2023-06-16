#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILEPATH "/tmp/chatbot.txt"
#define MAX_LENGTH 100


void sendmsg(const char* msg){
    FILE* file = fopen(FILEPATH, "a");
    if(file == NULL){
        printf("Error reading file");
        return 0;
    }
    fprintf(file, "%s\n", msg);
    fclose(file);
}

void receivemsg(const char* msg){
    FILE* file = fopen(FILEPATH, "a");
    if(file == NULL){
        printf("Error reading file");
        return 0;
    }
    fgets(msg, MAX_LENGTH, file);
    fclose(file);
}

int main(){
    char msg[MAX_LENGTH];
    pid_t pid = fork();
    if(pid < 0){
        printf("fork error");
        return 0;
    } else if (pid == 0){
        while(1){
            printf("message to send: ");
            fgets(msg,MAX_LENGTH,stdin);
            sendmsg(msg);
        }
    } else {
        while(1){
            receivemsg(msg);
            printf("received msg: %s\n",msg);
        }
    }
    return 0;
}
