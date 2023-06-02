#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L
#define BUFFER_SIZE 1024
#define LOG_FILE "server.log"
#define WEB_ROOT "./web"

/* Error handler */
void error(char *msg) {
    perror(msg);
    exit(1);
}

/* Signal handler */
void sigintHandler(int signal) {
    if (signal == SIGINT) {
        printf("\nShutting down server...\n");
    }
}

/* Log function */
void logWrite(const char *client_ip, int client_port, const char *filename) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        printf("Failed to open log file\n");
        return;
    } else {
        fprintf(log_file, "Client IP: %s, Port: %d, Requested file name: %s\n", client_ip, client_port, filename);
        fclose(log_file);
    }
}

/* Send file to client */
void sendFile(int sockfd, const char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(filename, "rb");
    int file_size = -1, bytes_read, bytes_sent;
    if (!file) {
        perror("Error opening file");
        send(sockfd, &file_size, sizeof(file_size), 0);
        return;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    send(sockfd, &file_size, sizeof(file_size), 0);

    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        bytes_sent = send(sockfd, buffer, bytes_read, 0);
        if (bytes_sent < 0) {
            perror("Error sending file");
            break;
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    /* Variable initialization */
    int sockfd = -1;
    int server_port = -1;
    int client_port = -1;
    int filename_length = -1;
    struct sockaddr_in server_address;
    char client_ip[INET_ADDRSTRLEN], filename[BUFFER_SIZE], filepath[BUFFER_SIZE + sizeof(WEB_ROOT)];

    /* Port validation */
    if (argc < 2) {
        fprintf(stderr, "ERROR: No port provided\n");
        exit(1);
    }
    server_port = atoi(argv[1]);

    /* Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket\n");
        exit(1);
    }

    /* Server struct configuration */
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    /* Binding socket with server address */
    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        error("Error on binding\n");
    }

    /* Checking for listen errors */
    if (listen(sockfd, 5) < 0) {
        error("ERROR on listening\n");
    }

    /* Ctrl+C signal handler */
    signal(SIGINT, sigintHandler);

    /* Printing port info */
    printf("Server is running and listening on port %d\n", server_port);

    /* Server loop for listening to connections */
    while (1) {
        /* Creating client struct */
        struct sockaddr_in client_address;
        socklen_t clientlength = sizeof(client_address);

        /* Initializing connection between client and server socket */
        int newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &clientlength);
        if (newsockfd < 0) {
            error("Error on accepting connection\n");
        }

        /* Configuring client info */
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
        client_port = ntohs(client_address.sin_port);

        /* Handling received info about file and its length */
        if (recv(newsockfd, &filename_length, sizeof(filename_length), 0) < 0) {
            error("ERROR receiving filename length");
        }

        if (recv(newsockfd, filename, filename_length, 0) < 0) {
            error("ERROR receiving filename");
        }
        filename[filename_length] = '\0';

        /* Creating log of file requested from server with client info */
        logWrite(client_ip, client_port, filename);

        /* Send requested file to client */
        strcpy(filepath, WEB_ROOT);
        strcat(filepath, "/");
        strcat(filepath, filename);
        sendFile(newsockfd, filepath);

        /* Close client socket */
        close(newsockfd);
    }

    /* Close server socket */
    close(sockfd);
    return 0;
}
