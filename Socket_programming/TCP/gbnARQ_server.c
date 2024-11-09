// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<time.h>
#include <arpa/inet.h>

#define PORT 8080
#define PACKET_SIZE 1024
#define TOTAL_PACKETS 8

// Simulate random packet loss (0 = no loss, 1 = loss)
int simulate_packet_loss() {
    return rand() % 5 == 0;  // 20% chance of loss
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[PACKET_SIZE] = {0};
    int expected_seq = 0;
    
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

        if (seq_num == expected_seq) {
            printf("[Receiver] Packet %d received in sequence.\n", seq_num);
            expected_seq++;
        } else {
            printf("[Receiver] Out-of-sequence packet %d received, expecting %d. Discarding packet.\n", seq_num, expected_seq);
        }

        // Send cumulative ACK for the last in-sequence packet
        char ack[PACKET_SIZE];
        sprintf(ack, "ACK %d", expected_seq - 1);
        send(new_socket, ack, strlen(ack), 0);
        printf("[Receiver] Cumulative ACK %d sent.\n", expected_seq - 1);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
