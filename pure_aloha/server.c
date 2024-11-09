#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    unsigned char buffer[BUFFER_SIZE] = {0};

    // Creating a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setting up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the specified port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding to port failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections...\n");

    // Accepting a new client connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accepting new request failed");
        exit(EXIT_FAILURE);
    }

    // Receiving data bits one by one
    while (1) {
        int bytes_read = read(new_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break; // Exit on read error or connection closure
        }

        // Print the received data bit
        printf("Received data bit: %d\n", buffer[0]);
    send(new_socket, "Data received successfully.\n", 30, 0);
  
        
    }

    // Close sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
