// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<time.h>

#define PORT 8080
#define PACKET_SIZE 1024
#define TOTAL_PACKETS 8

// Simulate random packet loss or corruption
int simulate_packet_loss() {
    return rand() % 5 == 0;  // 20% chance of packet loss or corruption
}

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[PACKET_SIZE] = {0};
    int window_size, seq_num = 0;
    int last_acked = -1;
    int sent[TOTAL_PACKETS] = {0}; // Track sent packets
    int acked[TOTAL_PACKETS] = {0}; // Track received ACKs

    srand(time(0));  // Seed for random packet loss

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Input window size
    printf("Enter window size: ");
    scanf("%d", &window_size);

    // Start Selective Repeat ARQ protocol
    while (last_acked < TOTAL_PACKETS - 1) {
        // Send all packets within window size
        for (int i = seq_num; i < seq_num + window_size && i < TOTAL_PACKETS; i++) {
            if (!sent[i]) {
                sprintf(buffer, "Packet %d", i);
                if (!simulate_packet_loss()) {
                    printf("[Sender] Sending %s\n", buffer);
                    send(sock, buffer, strlen(buffer), 0);
                } else {
                    printf("[Sender] Packet %d lost!\n", i);
                }
                sent[i] = 1;  // Mark packet as sent
            }
        }

        // Wait for acknowledgment
        read(sock, buffer, PACKET_SIZE);
        int ack_num;
        sscanf(buffer, "ACK %d", &ack_num);
        printf("[Sender] Received %s\n", buffer);

        // Mark the acknowledged packet
        acked[ack_num] = 1;

        // Move window forward based on received ACKs
        while (acked[seq_num]) {
            last_acked = seq_num;
            seq_num++;
        }

        sleep(1);  // Simulate transmission delay
    }

    // Ensure the last packet's ACK is received before closing
    while (last_acked < TOTAL_PACKETS - 1) {
        read(sock, buffer, PACKET_SIZE);
        int ack_num;
        sscanf(buffer, "ACK %d", &ack_num);
        printf("[Sender] Received final ACK %d\n", ack_num);

        if (ack_num == TOTAL_PACKETS - 1) {
            last_acked = TOTAL_PACKETS - 1;
        }
    }

    printf("[Sender] All packets sent and acknowledged. Closing connection.\n");

    close(sock);
    return 0;
}

