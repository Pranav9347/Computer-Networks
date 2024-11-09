#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    unsigned char data_bit; // Single data bit
    char buffer[BUFFER_SIZE] = {0};
    int data_length = 10; // Total bits to send

    // Create the socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Setup the server address struct
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert the IPv4 address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection to server failed\n");
        return -1;
    }

    // Send data bits one by one
     int collisions = 0;
    for (int i = 0; i < data_length;) {
        //data_bit = rand() % 256; // Generate a random data bit (0-255)
        data_bit = i + 1;
        int collision = rand() % 2; // Randomly decide if a collision occurs (50% chance)
        if(collision)
        {
            printf("Collision occured!!\n Retransmitting.... \n\n");
            collisions++;
            continue;
        }
        else
        {
            printf("Number of Collsions occured for this data:%d\n",collisions);
            collisions = 0;
            send(sock, &data_bit, sizeof(data_bit), 0); // Send the data bit
            printf("Sent data bit: %d\n", data_bit);
            i++;
        }
        

        // Wait for acknowledgment from the server
        read(sock, buffer, BUFFER_SIZE);
        printf("Server response: %s\n", buffer);
    }

    // Close the connection
    close(sock);
    return 0;
}
