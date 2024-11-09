#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() 
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	char buffer[1024] = {0};
	char *message = "Hello from Server!";
	socklen_t addrlen = sizeof(address);
	
//1. Creating socket file descrpitor:
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) 
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

//2. Define the server address
	address.sin_family = AF_INET;//IPv4
	address.sin_addr.s_addr = INADDR_ANY;//Any incoming IP address

	address.sin_port = htons(PORT); //convert port to network byte order

//3. Bind the socket to the specified IP and port 
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address)) < 0)
	{
		perror("Bind failed");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	
//4. Listen for incoming connections(backlog size of 3)
	  if (listen(server_fd, 3) < 0) 
	  {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
      }
      
      printf("TCP server waiting for a connection...\n");
      
 //5. Accept an incoming connection
 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) 
    {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
//6. Receive data from the client:
	read(new_socket, buffer, BUFFER_SIZE);
    printf("Message from client: %s\n", buffer);

//7. Send a message to the client
    send(new_socket, message, strlen(message), 0);
    printf("message sent to client\n");

//8. Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}
