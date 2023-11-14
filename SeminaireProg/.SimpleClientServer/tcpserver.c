#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 4455

int main(){
    int sockfd;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    
    char buffer[1024]; 

    //Create the server socket
    sockfd=socket(AF_INET, SOCK_STREAM, 0); //you might find PF_INET too
    printf("\nServer socket created\n");
    //memset() is used to fill the structure with 0
    memset(&serverAddr, '\0', sizeof(serverAddr));

   
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("\nBinded to port %d\n", 4455);

    listen(sockfd, 5); //5 clients can connect to me
    printf("Listening...\n");

    addr_size=sizeof(newAddr);

    newSocket=accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);

    strcpy(buffer, "\nHello from server\n");
    
    send(newSocket, buffer, strlen(buffer), 0);
    printf("Sending the data to the client");

    close(newSocket);
     

     return 0;

}
