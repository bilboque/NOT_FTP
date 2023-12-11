#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
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
        
        pid_t pid = fork();
        if(pid == 0){
            pid_t cpid = fork();

            if(cpid == 0){
                
                char nsockbuff[10];
                memset(nsockbuff, '\0', 10);
                snprintf(nsockbuff, 10, "%d", newSocket);

                char * pargv[] = {"./out/server", nsockbuff, NULL};

                int ret = execv(pargv[0], pargv);
                if(ret == -1){
                    perror("execv");
                }
            }
            else{
                exit(EXIT_SUCCESS);
            }
        }
        else if(pid > 0){
            wait(NULL);
            close(newSocket);
        }
        else{
            HANDLE_ERROR("fork");
        }
    }
}
