#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD_CHAR 64

#define DEFAULT_CMD "help"
#define DEFAULT_FILE "default.txt"

#define HELP "help"
#define HELP_DSC "print this help"

#define LIST "list"
#define LIST_DSC "list available server files"

#define GET "get"
#define GET_DSC "get a server file"

#define QUIT_DSC "quit client"

#define N_CMD 4

#define HANDLE_GETCMD_ERROR {fprintf(stderr, "unexpected unexpected number of arguments returned by sscanf\n"); exit(EXIT_FAILURE);}
#define PRINT_CMD(a) printf("cmd: %s, file: %s\n", a->type, a->file)
#define CMD_NOMATCH(cmd) printf("Could not match cmd\n")

void help(){
    puts("HELP:");
    printf("\t%s: %s\n", HELP, HELP_DSC);
    printf("\t%s: %s\n", LIST, LIST_DSC);
    printf("\t%s: %s\n", GET, GET_DSC);
    printf("\t%s: %s\n", QUIT, QUIT_DSC);
}

cmd * init_cmd(){
    cmd * n_cmd = (cmd *) malloc(sizeof(cmd));
    
    n_cmd->file = DEFAULT_CMD;
    n_cmd->type = DEFAULT_FILE;

    return n_cmd;
}

void check_cmd(cmd * cmd){
    char ** valid_cmd = (char **) malloc(sizeof(char * ) * N_CMD);
    
    valid_cmd[0] = HELP;
    valid_cmd[1] = LIST;
    valid_cmd[2] = GET;
    valid_cmd[3] = QUIT;
    
    int valid = 0;
    
    for(int i = 0; i<N_CMD; i++){
        if(strcmp(cmd->type, valid_cmd[i]) == 0){
            valid = 1;
        }
    }

    if(valid){
        PRINT_CMD(cmd);
        free(cmd);
        //send_cmd();
    }
    else{
        PRINT_CMD(cmd);
        CMD_NOMATCH(cmd);
        free(cmd);
    }
}

void get_cmd(cmd * cmd){
    char * stdin_cmd = malloc(sizeof(char) * MAX_CMD_CHAR);
    printf("type a cmd:\n> ");
    fflush(stdin);
    fgets(stdin_cmd, MAX_CMD_CHAR, stdin);

    char * type = malloc(sizeof(char) * 10);
    char * file = malloc(sizeof(char) * 64);
    int n_arg = sscanf(stdin_cmd, "%s %s\n", type, file);
    switch (n_arg) {
        case 1:
            cmd->type = type;
            cmd->file = DEFAULT_FILE;
            break;
        case 2:
            cmd->type = type;
            cmd->file = file;
            break;
        default:
            HANDLE_GETCMD_ERROR;
            break;
    }
    check_cmd(cmd);
}
