#include "protocole.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>

#define PORT 4455
#define IP_ADDRESS "127.0.0.1"
#define HANDLE_ERROR(msg) { perror(msg); exit(EXIT_FAILURE); }
#define STORAGE_PATH "./myBigDataCenter/"

int main(){
    int sockfd;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    sockfd=socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        HANDLE_ERROR("socket");
    }
    printf("\nServer socket created\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr=inet_addr(IP_ADDRESS);

    if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        HANDLE_ERROR("bind");
    }

    printf("\nIP adresse: %s\n", IP_ADDRESS);
    printf("Binded to port %d\n", PORT);

    listen(sockfd, 5);
    printf("Listening...\n");

    addr_size=sizeof(newAddr);
    while(1){
        newSocket=accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        
        int pid = fork();
        if(pid == 0){
            int cpid = fork();

            if(cpid == 0){
                char buffer[MAX_READ_LEN];

                strcpy(buffer, "Hello from server");
                write(newSocket, buffer, MAX_READ_LEN);
                printf("Sending the data to the client\n");

                char path[MAX_PATH_LEN] = "N/A";
                int cmd = CMD_DEFAULT;

                while(1){
                    read(newSocket, buffer, MAX_READ_LEN);
                    sscanf(buffer, "cmd=%d, file=%s", &cmd, path);
                    PRINT_MSG(cmd, path);

                    switch (cmd) {
                        case CMD_EXIT:
                            server_exit(newSocket);
                            break;
                        case CMD_LIST:
                            send_list(newSocket, STORAGE_PATH);
                            break;
                        case CMD_GET:
                            server_get(newSocket, STORAGE_PATH, path);
                            break;
                        case CMD_PUT:
                            server_put(newSocket, STORAGE_PATH, path);
                            break;
                        default: // Impossible
                            ALERT_UNEXPECTED_EVENT("Unknown command from client", newSocket);
                            break;
                    }
                }
            }
            else{
                exit(EXIT_SUCCESS);
            }
        }
        else if(pid > 0){
            wait(NULL);
        }
        else{
            HANDLE_ERROR("fork");
        }
    }
}
