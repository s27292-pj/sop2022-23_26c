#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define _XOPEN_SOURCE 700
#define BUFFER_SIZE 256
#define LOG_FILE "server.log"


/* error handler */
void error(char *msg){
    perror(msg);
    exit(1);
}

/* signal handler */
void sigintHandler(int signal){
    if(signal == SIGINT){
        printf("\n Shutting down server... \n");
    }
}

/* log function */
void logWrite(const char *client_ip, int client_port, const char *filename){
    FILE *log_file = fopen(LOG_FILE,"a");
    if(!log_file){
        printf("Failed to open log file\n");
        return;
    } else {
        fprintf(log_file,"Client IP: %s, Port: %d, Requested file name: %s\n",client_ip,client_port,filename);
    }
}


int main(int argc, char *argv[]){

    /* var init */
    int server_port, sockfd, newsockfd, filename_length, client_port, received_info, file_size, sent_info, remaining_file_size,remaining_file_size_to_send,read;
    char filename[BUFFER_SIZE],client_ip[INET_ADDRSTRLEN], file_buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    FILE *file;

    /* port init */
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    } else {
        server_port = atoi(argv[1]);
    }
    
    /* socket init */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket\n");
        exit(1);
    }

    /* server struct config */
    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    /* binding socket with server address */
    if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        error("Error on binding\n");
    }

    /* checking for listen errors */
    if(listen(sockfd, 5) < 0){
        error("ERROR on listening\n");
    }
    
    /* ctrl+c signal handler */
    signal(SIGINT,sigintHandler);

    /* printing port info */
    printf("Server is running and listening on port %d\n", server_port);
    
    /* server loop for listening to connections */
    while(1){

        /* creating client struct */
        struct sockaddr_in client_address;
        socklen_t clientlength = sizeof(client_address);

        /* initializing connection between client and server socket */
        newsockfd = accept(sockfd,(struct sockaddr *) &client_address, &clientlength);
        if(newsockfd < 0 ){
            error("Error on accepting connection\n");
        }

        /* configuring client info */
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
        client_port = ntohs(client_address.sin_port);

        /* handling received info about file and it's length */
        received_info = recv(newsockfd,&filename_length,sizeof(filename_length),0);
        if(received_info < 0){
            error("ERROR receiving filename length\n");
        }
        received_info = recv(newsockfd, filename, filename_length, 0);
        if (received_info < 0) {
            error("ERROR receiving filename");
        } else { 
            filename[received_info] = '\0';
        }

        /* creating log of file requested from server with client info */
        logWrite(client_ip,client_port,LOG_FILE);

        /* requested file handling */
        file = fopen(filename,"rb");
        if(!file){

            /* handling when file is not on server */
            file_size = -1;
            sent_info = send(newsockfd,&file_size,sizeof(file_size),0);
            if (sent_info < 0){
                error("ERROR sending file size\n");
            }
        } else {
            
            /* handling when file is found */
            fseek(file, 0, SEEK_END);
            file_size = ftell(file);
            rewind(file);
            sent_info = send(newsockfd, &file_size, sizeof(file_size),0);
            if(sent_info < 0){
                error("ERROR sending file size");
            }
            remaining_file_size = file_size;

            while(remaining_file_size > 0){
                remaining_file_size_to_send = (remaining_file_size < BUFFER_SIZE) ? remaining_file_size : BUFFER_SIZE;
                read = fread(file_buffer, sizeof(char), remaining_file_size_to_send, file);
                if (read < remaining_file_size_to_send) {
                    error("ERROR reading from file");
                }

                sent_info = send(newsockfd, file_buffer, read, 0);
                if (sent_info < 0) {
                    error("ERROR sending file data");
                }

                remaining_file_size -= sent_info;
            }
            fclose(file);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}
