#include "opt.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
 
ap * init_ap_from_stdin(){
    ap * new_conn = malloc(sizeof(ap));

    new_conn->ip = malloc(sizeof(char) * 16);
    new_conn->port = 0;
    
    printf("Enter Server IP adress :\n> ");
    if(scanf("%s",new_conn->ip) == -1){
        perror("scanf");
        exit(EXIT_SUCCESS);
    }

    if(strlen(new_conn->ip)>15){
        fprintf(stderr, "IPv4 ADRESS INVALID\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter Server Port :\n> ");
    if(scanf("%d",&new_conn->port) == -1){
        perror("scanf");
        exit(EXIT_SUCCESS);
    }
    getchar();

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
