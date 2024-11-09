#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char *hello = "Hello from UDP server";
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);
    printf("UDP server waiting for a message...\n");

    // Receive message from client
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Message from client: %s\n", buffer);

    // Send message to client
    sendto(sockfd, hello, strlen(hello), 0, (struct sockaddr *)&cliaddr, len);
    printf("Hello message sent to client\n");

    // Close the socket
    close(sockfd);
    return 0;
}

