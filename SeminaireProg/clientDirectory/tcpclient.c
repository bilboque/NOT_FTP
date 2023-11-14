#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>


#define HANDLE_ERROR(msg) { perror(msg); exit(EXIT_FAILURE); }

typedef struct address_and_port{
    char * ip;
    int port;
} ap;

ap * init_ap_from_stdin(){
    ap * new_conn = malloc(sizeof(ap));

    new_conn->ip = malloc(sizeof(char) * 16);
    new_conn->port = 0;
    
    printf("Enter Server IP adress :\n> ");
    if(scanf("%s",new_conn->ip) == -1){
        HANDLE_ERROR("scanf ");
    }

    if(strlen(new_conn->ip)>15){
        fprintf(stderr, "IPv4 ADRESS INVALID\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter Server Port :\n> ");
    if(scanf("%d",&new_conn->port) == -1){
        HANDLE_ERROR("scanf");
    }
    

    return new_conn;
}

ap * init_ap_from_argv(int argc, char * argv[]){

    ap * new_conn = malloc(sizeof(ap));

    new_conn->ip = argv[1];


    int port = 0;
    int factor = 1;

    for(int i = strlen(argv[2]) -1 ; i>-1;i--){
        int currentInt = argv[2][i] - '0';

        if(currentInt < 10 && currentInt > -1){
            port += currentInt * factor;
            factor *= 10;
        }
        else{
            fprintf(stderr, "INVALID PORT : argv[2][%d] = %c\n" , i , argv[2][i]);
            exit(EXIT_FAILURE);
        }
    }

    new_conn->port = port; 
    
    if(strlen(new_conn->ip)>15){
        fprintf(stderr, "IPv4 ADRESS INVALID\n");
        exit(EXIT_FAILURE);
    }


    return new_conn;
}

ap * GET_AP(int argc, char * argv[]){
    return (argc==3) ? init_ap_from_argv(argc,argv) : init_ap_from_stdin();
}

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

    if(close(clientSocket)==-1){
        HANDLE_ERROR("close ");
    }

    printf("Closing the connection\n");

    exit(EXIT_SUCCESS);
}
