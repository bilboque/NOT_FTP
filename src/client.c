#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "opt.h"
#include "protocole.h"

int main (int argc, char * argv[]){
    ap * addr_n_port = GET_AP(argc,argv);

    int clientSocket;
    struct sockaddr_in serverAddr;

    //Create the client socket
    clientSocket=socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1){
        HANDLE_ERROR("socket");
    }
    printf("Client socket created succesfully \n");
    
    memset(&serverAddr, '\0', sizeof(serverAddr)); // LEGACY CODE

    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(addr_n_port->port);
    serverAddr.sin_addr.s_addr=inet_addr(addr_n_port->ip);

    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr))==-1){
        HANDLE_ERROR("connnect "); 
    }

    char path_buff[MAX_PATH_LEN];
    int cmd = CMD_DEFAULT;
    char buffer[MAX_READ_LEN]; 

    printf("\n-> Connected to the Server <-\n");

    read(clientSocket, buffer, MAX_READ_LEN);
    printf("Greatings from server -> %s\n", buffer);

    while (cmd != CMD_EXIT){
        memset(path_buff, '\0', MAX_PATH_LEN);

        get_cmd(&cmd, path_buff);
        
        switch (cmd) {
            case CMD_LIST:
                rcv_list(clientSocket);
                break;
            case CMD_GET:
                client_get(clientSocket, path_buff);
                break;
            case CMD_PUT:
                client_put(clientSocket, path_buff);
                break;
            case CMD_HELP:
                help();
                break;
            case CMD_EXIT:
                client_exit(clientSocket);
                break;
            default:
                fprintf(stderr,"commande inconnue\n");
                break;
        }
    }

    if(close(clientSocket)==-1){
        HANDLE_ERROR("close ");
    }

    printf("Closing the connection\n");

    exit(EXIT_SUCCESS);
}
