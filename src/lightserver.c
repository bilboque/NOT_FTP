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
    
    int cl_id = 0;
    while(1){
        newSocket=accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        cl_id++;

        pid_t pid = fork();
        if(pid == 0){
            pid_t cpid = fork();

            if(cpid == 0){
                
                char nsockbuff[20];
                char nclbuff[20];
                memset(nsockbuff, '\0', 20);
                memset(nclbuff, '\0', 20);
                snprintf(nsockbuff, 20, "%d", newSocket);
                snprintf(nclbuff, 20, "%d", cl_id);

                char * pargv[] = {"./out/server", nsockbuff, nclbuff, NULL};

                int ret = execv(pargv[0], pargv);
                if(ret == -1){
                    HANDLE_ERROR("execv");
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
