// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>   // For fd_set and select()
#include <errno.h>

#define PORT 8080
#define PACKET_SIZE 1024
#define TIMEOUT 3  // Timeout duration in seconds

int main() {
    struct sockaddr_in serv_addr;
    int sock = 0;
    char buffer[PACKET_SIZE] = {0};
    char ack_buffer[PACKET_SIZE] = {0};
    char packet[PACKET_SIZE];
    int total_packets, i = 0;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Set server address and port
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

    // Input number of packets to send
    printf("Enter total number of packets to send: ");
    scanf("%d", &total_packets);

    while (i < total_packets) {
        sprintf(packet, "Packet %d", i + 1);
        printf("[Sender] Sending %s...\n", packet);
        send(sock, packet, strlen(packet), 0);  // Send packet
        sleep(1);  // Simulate transmission delay

        // Wait for ACK with timeout
        fd_set fds;
        struct timeval timeout;
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        int ack_status = select(sock + 1, &fds, NULL, NULL, &timeout);

        if (ack_status > 0 && FD_ISSET(sock, &fds)) {
            read(sock, ack_buffer, PACKET_SIZE);
            if (strcmp(ack_buffer, "ACK") == 0) {
                printf("[Sender] ACK received for %s\n", packet);
                i++;  // Move to next packet
            }
        } else {
            printf("[Sender] ACK not received for %s, retransmitting...\n", packet);
        }
    }

    close(sock);
    return 0;
}
