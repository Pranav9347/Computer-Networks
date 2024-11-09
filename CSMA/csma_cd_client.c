#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    unsigned char data_bit;
    char buffer[BUFFER_SIZE];
    int data_length = 10;  // Total bits to send
    srand(time(0));  // Seed for random delays
    
    // Create the socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }
    
    // Set up the server address struct
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert the IPv4 address to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address / Address not supported\n");
        return -1;
    }
    
    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }
    
    // Simulate sending data bits one by one
    for (int i = 0; i < data_length; ++i) {
        data_bit = i + 1;  // Increment the data value
        
        // Send the data bit
        send(sock, &data_bit, sizeof(data_bit), 0);
        printf("Sent data bit: %d\n", data_bit);
        
        // Wait for server's response
        int bytes_read = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the buffer
            printf("Server response: %s\n", buffer);
        }
        
        // Simulate random delay between transmissions to force collisions
        usleep((rand() % 500 + 1) * 1000);  // Random delay between 1 and 500 ms
    }
    
    close(sock);
    return 0;
}
