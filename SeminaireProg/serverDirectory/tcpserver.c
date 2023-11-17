#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 4455
#define IP_ADDRESS "127.0.0.1"
#define HANDLE_ERROR(msg) { perror(msg); exit(EXIT_FAILURE); }

int main(){
    int sockfd;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    
    char buffer[1024]; 

    //Create the server socket
    sockfd=socket(AF_INET, SOCK_STREAM, 0); //you might find PF_INET too
    if(sockfd == -1){
        HANDLE_ERROR("socket ");
    }
    printf("\nServer socket created\n");

    //memset() is used to fill the structure with 0
    memset(&serverAddr, '\0', sizeof(serverAddr));

   
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=inet_addr(IP_ADDRESS);


    if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        HANDLE_ERROR("bind ");
    }

    printf("\nIP adresse: %s\n",IP_ADDRESS);
    printf("Binded to port %d\n", PORT);

    listen(sockfd, 5); //5 clients can connect to me
    printf("Listening...\n");

    addr_size=sizeof(newAddr);

    newSocket=accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);

    strcpy(buffer, "Hello from server");
    send(newSocket, buffer, strlen(buffer) + 1 , 0);
    printf("Sending the data to the client\n");

    if(close(newSocket)==-1){
        HANDLE_ERROR("close ");
    }
    if(close(sockfd)==-1){
        HANDLE_ERROR("close ");
    }
    printf("Closing the connection\n");
    exit(EXIT_SUCCESS);
}
