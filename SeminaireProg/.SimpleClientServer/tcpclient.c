#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 4455

int main (){

    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024]; 

    //Create the client socket
    clientSocket=socket(AF_INET, SOCK_STREAM, 0);

    printf("Client socket created succesfully \n");

    //memset() is used to fill the structure with 0
    memset(&serverAddr, '\0', sizeof(serverAddr));

    //Identifying the family, port and address
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //Connecting  to the server socket
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to the Server\n");

    recv(clientSocket, buffer, 1024, 0);
    printf("Data Received: %s", buffer);
    
    close(clientSocket);
    printf("Closing the connection");

    return 0;
}
