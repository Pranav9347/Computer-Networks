//tcpserver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	char buffer[MAX_SIZE];
	char* message="Hello from Server!";
	int addr_len = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;

	//1. Create a socket
	if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("Unable to create a socket!\n");
		exit(EXIT_FAILURE);
	}

	//2. Bind the socket
	if(bind(server_fd,(struct sockaddr*)&address,addr_len)<0)
	{
		perror("Unable to bind the socket!\n");
		exit(EXIT_FAILURE);
	}
	//3. Listen
	if(listen(server_fd,3)<0)
	{
		perror("Unable to listen!\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Server listening on port %d\n",PORT);
	//4. Accept
	if((new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addr_len))<0)
	{
		perror("Unable to accept!\n");
		exit(EXIT_FAILURE);
	}
	//5. Send/recv
	send(new_socket,message,strlen(message),0);
	recv(new_socket,buffer,MAX_SIZE,0);
	printf("Client says: %s\n",buffer);

	//6. Close
	close(server_fd);
	close(new_socket);
	return 0;
}
