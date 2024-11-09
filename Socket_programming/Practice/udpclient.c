#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[MAX_SIZE];
    char* message = "Hi from client!";

    memset(&serv_addr,0,sizeof(serv_addr));

    // Define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    

    //1. Create socket:
    if((client_fd = socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("Unable to create socket!");
        exit(EXIT_FAILURE);
    }
    
    //3. sendto/recv:
    sendto(client_fd,message,strlen(message),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    int n = recvfrom(client_fd,buffer,MAX_SIZE,0,NULL,NULL);
    buffer[n] = '\0';
    printf("Message from server: %s\n",buffer);
    close(client_fd);
    return 0;
}