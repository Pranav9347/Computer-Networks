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
	int server_fd, new_socket;
	struct sockaddr_in address;
	char* message="Hi from server!";
	char buffer[MAX_SIZE];
	int addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//Create a socket:
	if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("unable to create socket");
		exit(EXIT_FAILURE);
	}
	//bind:
	if((bind(server_fd,(struct sockaddr*)&address,addrlen))<0)
	{
		perror("unable to bind");
		exit(EXIT_FAILURE);
	}
	//listen:
	if((listen(server_fd,3))<0)
	{
		perror("unable to listen");
		exit(EXIT_FAILURE);
	}
	//accept:
	if((new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen))<=0)
	{
		perror("unable to accept");
		exit(EXIT_FAILURE);
	}
	send(new_socket,message,strlen(message),0);
	recv(new_socket,buffer,MAX_SIZE,0);
	printf("Client says: %s\n",buffer);
	return 0;
}