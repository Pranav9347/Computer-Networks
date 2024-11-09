// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<time.h>

#define PORT 8080
#define PACKET_SIZE 1024
#define TOTAL_PACKETS 8
#define WINDOW_SIZE 4

// Simulate random packet loss (0 = no loss, 1 = loss)
int simulate_packet_loss() {
    return rand() % 5 == 0;  // 20% chance of loss
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[PACKET_SIZE] = {0};
    int received[TOTAL_PACKETS] = {0}; // Track received packets
    int expected_seq = 0;  // Expected sequence number (starts from 0)

    srand(time(0));  // Seed for random packet loss

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connection...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connection established!\n");

    // Start receiving packets
    while (expected_seq < TOTAL_PACKETS) {
        read(new_socket, buffer, PACKET_SIZE);
        int seq_num;
        sscanf(buffer, "Packet %d", &seq_num);

        // Simulate packet loss or corruption
        if (simulate_packet_loss()) {
            printf("[Receiver] Packet %d lost!\n", seq_num);
            continue;
        }

        if (seq_num >= expected_seq && seq_num < expected_seq + WINDOW_SIZE && received[seq_num] == 0) {
            printf("[Receiver] Packet %d received.\n", seq_num);
            received[seq_num] = 1;

            // Send ACK for the received packet
            char ack[PACKET_SIZE];
            sprintf(ack, "ACK %d", seq_num);
            send(new_socket, ack, strlen(ack), 0);
            printf("[Receiver] ACK %d sent.\n", seq_num);
        } else {
            printf("[Receiver] Packet %d received out of order or duplicate, discarding.\n", seq_num);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

