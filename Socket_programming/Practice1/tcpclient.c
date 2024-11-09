//tcpclient.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
	int client_fd;
	struct sockaddr_in address;
	int addr_len = sizeof(address);
	char* message = "Hello from Client!";
	char buffer[MAX_SIZE];

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);

	//1. Create a socket
	if((client_fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("Couldn't create client socket\n");
		exit(EXIT_FAILURE);
	}
	//2. Convert IP to binary
	if(inet_pton(AF_INET,"127.0.0.1",&address.sin_addr)<=0)
	{
		perror("Invalid address\n");
		exit(EXIT_FAILURE);
	}
	//3. Connect to server
	if(connect(client_fd,(struct sockaddr*)&address,addr_len)<0)
	{
		perror("Couldn't connect to server!\n");
		exit(EXIT_FAILURE);
	}
	//4. Send/recv
		send(client_fd,message,strlen(message),0);
		recv(client_fd,buffer,MAX_SIZE,0);
		printf("Server says: %s\n",buffer);
	//5. Close
		close(client_fd);

	return 0;
}

