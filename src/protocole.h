#ifndef __PROTOCOLE_H__
#define __PROTOCOLE_H__

// Buffer Constants
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
#define CMD_CD 5

// Commands
#define QUIT "quit"
#define QUIT_DSC "quit client"
#define LIST "list"
#define LIST_DSC "list available server files"
#define GET "get"
#define GET_DSC "get a server file"
#define PUT "put"
#define PUT_DSC "send a file to server"
#define HELP "help"
#define HELP_DSC "print this help"
#define CD "cd"
#define CD_DSC "change client working directory"

#define ABORT "ABORT"
#define CTS "CTS"

// Comands utiliaire
#define N_CMD 6
#define VALIDCMD {QUIT, LIST, GET, PUT, HELP, CD}
#define VALID_DSC {QUIT_DSC, LIST_DSC, GET_DSC, PUT_DSC, HELP_DSC, CD_DSC}

// Debug and log purpose
#define PRINT_MSG(a,b) printf("cmd=%d, file=%s\n", a, b)

// Error handleing
#define HANDLE_ERROR(msg) { perror(msg); exit(EXIT_FAILURE); }
#define HANDLE_GET_FAILSAFE(sfd) {fprintf(stderr,"ABORT received (the file requested can't be opened or did not exist)\n");}
#define HANDLE_PUT_FAILSAFE(sfd) {fprintf(stderr,"ABORT received (the file already exists or can't be created)\n");}
#define ALERT_UNEXPECTED_EVENT(str,sfd){fprintf(stderr,"Unexpected Event: %s\n", str); write(sfd, ABORT, strlen(ABORT));}
#define HANDLE_UNEXPECTED_EVENT(str,sfd){fprintf(stderr,"Unexpected Event: %s\n", str);}


// utilitaire
void stf_send(int sfd, int ffd);
void fts_send(int sfd, int ffd); // Utilise le senfile syscall non-posix (voir le readme)
int string_to_int(char * snum);

// client side functions
void client_list(int sfd);
void client_get(int sfd, char * path, char * new_file_name);
void client_put(int sfd, char * path, char * new_file_name);
void client_exit(int sfd);
// builtins
void help();
void cd(char * wd);

int check_cmd(char * cmd);
int get_cmd(int * cmd, char * file, char * new_file);

// server side functions
void server_list(int sfd, char * path);
void server_get(int sfd, char * storage_dir, char * file_path);
void server_put(int sfd, char * storage_dir, char * file_path);
void server_exit(int sfd, int client_id);

#endif
