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

    // Start Go-Back-N ARQ protocol
    while (seq_num < TOTAL_PACKETS) {
        // Send packets in the window
        for (int i = 0; i < window_size && seq_num < TOTAL_PACKETS; i++) {
            if (!simulate_packet_loss()) {
                sprintf(buffer, "Packet %d", seq_num);
                printf("[Sender] Sending %s\n", buffer);
                send(sock, buffer, strlen(buffer), 0);
            } else {
                printf("[Sender] Packet %d lost!\n", seq_num);
            }
            seq_num++;
            sleep(1);  // Simulate transmission delay
        }

        // Wait for acknowledgment
        read(sock, buffer, PACKET_SIZE);
        int ack_num;
        sscanf(buffer, "ACK %d", &ack_num);
        printf("[Sender] Received %s\n", buffer);

        if (ack_num >= last_acked) {
            last_acked = ack_num;
            printf("[Sender] ACK received for all packets up to %d\n", last_acked);
        } else {
            printf("[Sender] ACK out-of-sequence, go back to packet %d\n", last_acked + 1);
            seq_num = last_acked + 1;  // Go back to the last unacknowledged packet
        }
    }

    close(sock);
    return 0;
}
