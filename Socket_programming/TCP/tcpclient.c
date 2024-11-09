#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
	int sock=0;
	struct sockaddr_in serv_addr;
	char* message = "Hello from Client!";
	char buffer[BUFFER_SIZE]={0};
	//1. Creating socket file descriptor:
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	//2. Define the server address:
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	//3. Convert IP address to binary form and store it in the serv_addr structure:
	if(inet_pton(AF_INET,"172.16.24.69",&serv_addr.sin_addr)<=0)
	{
		perror("Invalid address or address not supported");
		close(sock);
		exit(EXIT_FAILURE);
	}

	//4. Connect to the server:
	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		perror("Connection failed");
		close(sock);
		exit(EXIT_FAILURE);
	}
	
	// 5. Send the message to the server:
    send(sock, message, strlen(message), 0);
    printf("Message sent from client: %s\n", message);

    // 6. Read the server's response:
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Message from server: %s\n", buffer);

    // 7. Close the socket:
    close(sock);

	return 0;
}
