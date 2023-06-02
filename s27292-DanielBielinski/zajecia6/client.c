#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

/* error handler */
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    
    /* var init */
    int server_port, sockfd, filename_length, sentfile,file_size,received,to_receive,written,remaining;
    char file_name[BUFFER_SIZE],buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    char* server_ip;

    /* port and ip init */
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    } else {
        server_ip = argv[1];
        server_port = atoi(argv[2]);
    }

    /* socket init */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket\n");
    }

    /* server config */
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        error("ERROR converting IP address\n");
    }
    
    /* establishing connection with server */
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        error("ERROR connecting to server\n");
    }

    /* handling filename request */
    printf("Enter filename that you wish to download from server: ");
    fgets(file_name, BUFFER_SIZE, stdin);
    file_name[strcspn(file_name, "\n")] = '\0';
    filename_length = strlen(file_name);

    /* sending file name and it's length */
    sentfile = send(sockfd,&filename_length,sizeof(filename_length),0);
    if(sentfile < 0){
        error("ERROR sending file length\n");
    }
    sentfile = send(sockfd,file_name,filename_length,0);
    if(sentfile < 0){
        error("ERROR sending file name\n");
    }

    /* handling received file */
    received = recv(sockfd, &file_size, sizeof(file_size), 0);
    if (received < 0) {
        error("ERROR receiving file size");
    }
     if (file_size == -1) {
        printf("Server returned an error: File not found\n");
    } else {
        FILE *file = fopen(file_name, "wb");
        printf("File name: %s, File size: %d\n",file_name,file_size);
        if (!file) {
            error("ERROR opening file for writing");
        }

        remaining = file_size;

        while (remaining > 0) {
            to_receive = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
            received = recv(sockfd, buffer, to_receive, 0);
            if (received < 0) {
                error("ERROR receiving file data");
            }

            written = fwrite(buffer, sizeof(char), received, file);
            if (written < received) {
                error("ERROR writing to file");
            }

            remaining -= received;
        }

        fclose(file);
        printf("File downloaded successfully: %s\n", file_name);
    }
    close(sockfd);
    return 0;
}
