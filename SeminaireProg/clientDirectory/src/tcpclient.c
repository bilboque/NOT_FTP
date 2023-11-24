#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "options.h"
#include "cmd.h"

int main (int argc, char * argv[]){
    ap * addr_n_port = GET_AP(argc,argv);

    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024]; 

    //Create the client socket
    clientSocket=socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1){
        HANDLE_ERROR("socket");
    }
    printf("Client socket created succesfully \n");
    
    //memset() is used to fill the structure with 0
    memset(&serverAddr, '\0', sizeof(serverAddr));

    //Identifying the family, port and address
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(addr_n_port->port);
    serverAddr.sin_addr.s_addr=inet_addr(addr_n_port->ip);

    //Connecting  to the server socket
    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))==-1){
        HANDLE_ERROR("connnect "); 
    }

    printf("Connected to the Server\n");

    recv(clientSocket, buffer, 1024, 0);
    printf("Data Received: %s\n", buffer);
    cmd * cmd = init_cmd();

    while(strcmp(cmd->type, QUIT) != 0){
        get_cmd(cmd);
        write(clientSocket, cmd->type, 1024);
    }
    free(cmd);

    if(close(clientSocket)==-1){
        HANDLE_ERROR("close ");
    }

    printf("Closing the connection\n");

    exit(EXIT_SUCCESS);
}
