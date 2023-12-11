#include "protocole.h"

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

int main(int argc, char * argv[]){
    if (argv[1] == NULL){
        exit(EXIT_FAILURE);
    }
    
    int newSocket = string_to_int(argv[1]);
    if(newSocket == -1){
        exit(EXIT_FAILURE);
    }

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
            default:
                ALERT_UNEXPECTED_EVENT("Unknown command from client", newSocket);
                exit(EXIT_FAILURE);
                break;
        }
    }
}
