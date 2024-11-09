// server.c acts as receiver
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define PACKET_SIZE 1024

int simulate_loss_or_corruption() {
    // Simulate a 30% chance of packet loss or corruption
    return rand() % 10 < 3;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[PACKET_SIZE] = {0};
    char ack[] = "ACK";
    srand(time(0));

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for a connection...\n");

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int valread = read(new_socket, buffer, PACKET_SIZE);
        if (valread == 0) {
            break;  // Exit if no more packets
        }

        printf("Received: %s\n", buffer);

        // Simulate packet corruption or loss
        if (simulate_loss_or_corruption()) {
            printf("[Receiver] Packet lost or corrupted! No ACK sent.\n");
        } else {
            printf("[Receiver] Sending ACK for packet %s...\n", buffer);
            send(new_socket, ack, strlen(ack), 0);  // Send ACK
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
