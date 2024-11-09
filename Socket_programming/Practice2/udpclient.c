///udpclient.c
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
	struct sockaddr_in seraddr;
	char* message = "Hi from client!";
	char buffer[MAX_SIZE];
	socklen_t len=sizeof(seraddr);

	memset(&seraddr,0,sizeof(seraddr));

	seraddr.sin_family=AF_INET;
	seraddr.sin_addr.s_addr=INADDR_ANY;
	seraddr.sin_port = htons(PORT);

	//create:
	if((client_fd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("Unable to create socket!");
		exit(EXIT_FAILURE);
	}

	//sendto/revcfrom:
	sendto(client_fd,message,strlen(message),0,(const struct sockaddr*)&seraddr,len);
	int n= recvfrom(client_fd,buffer,MAX_SIZE,0,NULL,NULL);
	buffer[n] = '\0';
	
	printf("server says:%s\n",buffer);
	return 0;
}