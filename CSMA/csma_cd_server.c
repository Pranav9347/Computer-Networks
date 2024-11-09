#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int collision = 0; // Global flag to detect collisions

// Thread function to handle client connections
void* handle_client(void* socket_desc) {
    int new_socket = *(int*)socket_desc;
    unsigned char buffer[BUFFER_SIZE];
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        
        // Receive data from the client
        int bytes_read = recv(new_socket, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) {
            break;
        }
        
        // Simulate a 50% chance of collision
        int r = rand() % 2;
        if (r == 1) {
            collision = 1;  // Simulate a collision
        } else {
            collision = 0;  // No collision
        }

        // If a collision happens, inform the client
        if (collision == 1) {
            printf("Collision detected! Data: %d not received properly.\n", buffer[0]);
            send(new_socket, "Collision detected. Retrying...\n", 32, 0);
        } else {
            printf("Received data: %d\n", buffer[0]);
            send(new_socket, "Data received successfully.\n", 30, 0);
        }
    }
    
    close(new_socket);
    return 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Seed random number generator for collisions
    srand(time(0));
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is running and waiting for connections...\n");
    
    // Accept clients and create a thread for each
    pthread_t thread_id;
    while ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) >= 0) {
        pthread_create(&thread_id, NULL, handle_client, (void*)&new_socket);
    }
    
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    
    close(server_fd);
    return 0;
}
