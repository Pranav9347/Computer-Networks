#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8080
#define MAX_SIZE 1024

int main()
{
    int server_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t len;
    char buffer[MAX_SIZE];
    char* message = "Hi from server!";

    memset(&serv_addr,0,sizeof(serv_addr));
    memset(&cli_addr,0,sizeof(cli_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    

    //1. Create socket:
    if((server_fd = socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("Unable to create socket!");
        exit(EXIT_FAILURE);
    }
    //2. bind:
    if(bind(server_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
        perror("Unable to bind!");
        exit(EXIT_FAILURE);
    }
    len = sizeof(cli_addr);
    //3. sendto/recv:
    int n = recvfrom(server_fd,buffer,MAX_SIZE,0,(struct sockaddr*)&cli_addr,&len);
    buffer[n] = '\0';
    sendto(server_fd,message,strlen(message),0,(const struct sockaddr*)&cli_addr,len);
    
    printf("Message from server: %s\n",buffer);
    close(server_fd);
    return 0;
}