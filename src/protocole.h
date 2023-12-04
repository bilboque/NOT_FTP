#ifndef __CMD_H__
#define __CMD_H__

// Constants
#define MAX_CMD_LEN 256
#define MAX_READ_LEN 4096
#define MAX_PATH_LEN 64

// Protocole 
#define CMD_DEFAULT -1
#define CMD_EXIT 0
#define CMD_LIST 1
#define CMD_GET 2
#define CMD_PUT 3
#define CMD_HELP 4

// Commands
#define QUIT "quit"
#define QUIT_DSC "quit client"
#define LIST "list"
#define LIST_DSC "list available server files"
#define GET "get"
#define GET_DSC "get a server file"
#define PUT "put"
#define PUT_DSC "send file to server"
#define HELP "help"
#define HELP_DSC "print this help"

#define ABORT "ABORT"
#define CTS "CTS"

// Comands utiliaire
#define N_CMD 5
#define VALIDCMD {QUIT, LIST, GET, PUT, HELP}
#define VALID_DSC {QUIT_DSC, LIST_DSC, GET_DSC, PUT_DSC, HELP_DSC}

// Debug purpose
#define PRINT_MSG(a,b) printf("cmd=%d, file=%s\n", a, b)

// Error handleing
#define HANDLE_ERROR(msg) { perror(msg); exit(EXIT_FAILURE); }
#define HANDLE_NCTS_FAILSAFE(sfd) {fprintf(stderr,"No CTS confirmation or ABORT received\n"); close(sfd); exit(EXIT_FAILURE);}
#define ALERT_UNEXPECTED_EVENT(str,sfd){fprintf(stderr,"Unexpected Event: %s\n", str); write(sfd, ABORT, strlen(ABORT)); close(sfd); exit(EXIT_FAILURE);}
#define HANDLE_UNEXPECTED_EVENT(str,sfd){fprintf(stderr,"Unexpected Event: %s\n", str); close(sfd); exit(EXIT_FAILURE);}


// utilitaire
void stf_send(int sfd, int ffd);
void fts_send(int sfd, int ffd);

// client side functions
void rcv_list(int sfd);
void client_get(int sfd, char * path);
void client_put(int sfd, char * path);
void client_exit(int sfd);

void help();

int check_cmd(char * cmd);
void get_cmd(int * cmd, char * file);

// server side functions
void send_list(int sfd, char * path);
void server_get(int sfd, char * storage_dir, char * file_path);
void server_put(int sfd, char * storage_dir, char * file_path);
void server_exit(int sfd);

#endif
