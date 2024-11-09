//tcpclient.c
//tcpserver.c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
	int client_fd;
	struct sockaddr_in address;
	char* message="Hi from client!";
	char buffer[MAX_SIZE];
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);

	//Create a socket:
	if((client_fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("unable to create socket");
		exit(EXIT_FAILURE);
	}
	//convert ip to binary:
	if((inet_pton(AF_INET,"127.0.0.1",&address.sin_addr))<=0)
	{
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}
	//connect:
	if((connect(client_fd,(struct sockaddr*)&address,addrlen))<0)
	{
		perror("unable to connect");
		exit(EXIT_FAILURE);
	}
	send(client_fd,message,strlen(message),0);
	recv(client_fd,buffer,MAX_SIZE,0);
	printf("Client says: %s\n",buffer);
	return 0;
}