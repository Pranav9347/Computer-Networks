#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    char buffer[MAX_SIZE];//to store client's msg
    char* message = "Hello from server";

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //1. Create a socket
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }

    //2. Bind the socket
    if(bind(server_fd,(struct sockaddr*)&address,addr_len)<0)
    {
        perror("Error binding the socket\n");
        exit(EXIT_FAILURE);
    }

    //3. Listen to clients
    if(listen(server_fd,3)<0)
    {
        perror("Error listening\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server listening on port 8080..\n");
    
    //4. Accept incoming connections
    if((new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addr_len))<0)
    {
        perror("Couldn't accept any connection\n");
        exit(EXIT_FAILURE);
    }

    //5. Send/receive data
    send(new_socket, message, strlen(message), 0);
    printf("Message sent\n");

    recv(new_socket, buffer, MAX_SIZE, 0);
    printf("Client says: %s\n", buffer);

    //6. Close connection
    close(new_socket);
    close(server_fd);
    return 0;
}