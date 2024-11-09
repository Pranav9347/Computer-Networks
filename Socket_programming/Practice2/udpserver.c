///udpserver.c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
	int server_fd;
	struct sockaddr_in seraddr, cliaddr;
	char* message = "Hi from server!";
	char buffer[MAX_SIZE];
	socklen_t len=sizeof(cliaddr);

	memset(&seraddr,0,sizeof(seraddr));
	memset(&cliaddr,0,sizeof(cliaddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_addr.s_addr=INADDR_ANY;
	seraddr.sin_port = htons(PORT);

	//create:
	if((server_fd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("Unable to create socket!");
		exit(EXIT_FAILURE);
	}
	//bind:
	if((bind(server_fd,(struct sockaddr*)&seraddr,sizeof(seraddr)))<0)
	{
		perror("Unable to bind socket!");
		exit(EXIT_FAILURE);
	}
	//sendto/revcfrom:
	int n= recvfrom(server_fd,buffer,MAX_SIZE,0,(struct sockaddr*)&cliaddr,&len);
	buffer[n] = '\0';
	sendto(server_fd,message,strlen(message),0,(const struct sockaddr*)&cliaddr,len);
	
	
	printf("client says:%s\n",buffer);
	return 0;
}