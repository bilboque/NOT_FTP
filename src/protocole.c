#include "protocole.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/sendfile.h>

#include <readline/readline.h>

// Util
void stf_send(int sfd, int ffd){
    char buff[MAX_READ_LEN];
    int n = 1;
    memset(buff, '\0', MAX_READ_LEN);

    while(1){
        n = read(sfd,buff,MAX_READ_LEN);
        if(n == -1){
            HANDLE_ERROR("read");
        }
        else if(n > 1 && buff[n-1] == '\0' &&  buff[n-2] == '\xFF'){
            if(write(ffd, buff, n-2) == -1){
                HANDLE_ERROR("write");
            }
            return;
        }
        else{
            if(buff[n-1] != '\xFF'){
                if(write(ffd, buff, n) == -1){
                    HANDLE_ERROR("write");
                }
            }
            else{ // Handle an edge case if the file is a pdf
                if(write(ffd, buff, n-1) == -1){
                    HANDLE_ERROR("write");
                }
                char next = buff[n-1];
                while(next == '\xFF'){
                    const char xFF = '\xFF';
                    n = read(sfd, &next, 1);
                    if(n == -1){
                        HANDLE_ERROR("read");
                    }
                    else if(next == '\0'){
                        return;
                    }
                    else{
                        if(write(ffd, &xFF, 1) == -1){
                            HANDLE_ERROR("write");
                        }
                    }
                }
                if(write(ffd, &next, 1) == -1){
                    HANDLE_ERROR("write");
                }
            }
            memset(buff, '\0', MAX_READ_LEN);
        }
    }
}

void fts_send(int sfd, int ffd){
    int n = 1;

    while(n>0){
        n = sendfile(sfd, ffd, NULL, MAX_READ_LEN);
        if(n == -1){
            write(sfd, "ERROR IN FILE TRANSMISSION \xFF\0", strlen("ERROR IN FILE TRASMISSION \xFF\0"));
            HANDLE_ERROR("sendfile");
        }
    }

    if(write(sfd, "\xFF\0", 2) == -1){
        HANDLE_ERROR("write");
    }
}

int string_to_int(char * snum){
    int result = -1;
    sscanf(snum, "%d", &result);
    return result;
}

// Client
void help(){
    puts("\n->->-> HELP <-<-<-");
    printf("> %s                          // %s\n", HELP, HELP_DSC);
    printf("> %s                          // %s\n", LIST, LIST_DSC);
    printf("> %s serverfile [dst_filename] // %s\n", GET, GET_DSC);
    printf("> %s clientfile [dst_filename] // %s\n", PUT, PUT_DSC);
    printf("> %s                          // %s\n", EXIT, EXIT_DSC);
    printf("> %s [path]                     // %s\n\n", CD, CD_DSC);
}

int check_cmd(char * cmd){
    char * valid_cmd[] = VALIDCMD;
    for(int i = 0; i < N_CMD; i++){
        if(strcmp(cmd, valid_cmd[i]) == 0){
            return i;
        }
    }
    
    return CMD_DEFAULT;
}

int get_cmd(int * cmd, char * file, char * new_file){
    char command[MAX_CMD_LEN];
    char path[MAX_PATH_LEN];
    char n_path[MAX_PATH_LEN];

    char * input = readline("> ");
    int n = sscanf(input, "%s %s %s", command, path, n_path);
    free(input);

    *cmd = check_cmd(command);

    switch (n) {
        case 0:
            HANDLE_ERROR("sscanf (something went wrong)");
        case 1:
            break;
        case 2:
            strncpy(file, path, MAX_PATH_LEN);
            break;
        case 3:
            strncpy(file, path, MAX_PATH_LEN);
            strncpy(new_file, n_path, MAX_PATH_LEN);
            break;
        default: // how
            break;
    }

    return n;
}

void cd(char * path){
    char * home = getenv("HOME");
    if(path == NULL || strcmp(path, "~") == 0){
        if(chdir(home) == -1){
            perror("chdir");
        }
    }
    else if(chdir(path) == -1){
        perror("chdir");
    }
    char * cwd = getcwd(NULL, 0);
    puts(cwd);
    free(cwd);
}

void client_list(int sfd){
    char buff[MAX_READ_LEN];
    memset(buff, '\0', MAX_READ_LEN);

    snprintf(buff, MAX_READ_LEN, "cmd=%d, file=N/A", CMD_LIST);

    if(write(sfd, buff, MAX_READ_LEN) == -1){
        HANDLE_ERROR("write");
    }
    memset(buff, '\0', MAX_READ_LEN);

    if(read(sfd, buff, MAX_READ_LEN) == -1){
        printf("%s\n", buff);
        HANDLE_ERROR("read");
    }
    if(strncmp(buff, CTS, strlen(CTS)) != 0){
        HANDLE_GET_FAILSAFE(sfd);
        return;
    }
    memset(buff, '\0', MAX_READ_LEN);

    puts("\n->->-> Server Files <-<-<-");
    while(1){
        int n = read(sfd,buff,MAX_READ_LEN);
        if(n == -1){
            HANDLE_ERROR("read");
        }
        else if(buff[n-1] == '\0'){
            printf("%s", buff);
            printf("\n");
            return;
        }
        else{
            printf("%s", buff);
            memset(buff, '\0', MAX_READ_LEN);
        }
    }
}

void client_get(int sfd, char * path, char * file_rename){
    char buff[MAX_READ_LEN];
    memset(buff, '\0', MAX_READ_LEN);

    snprintf(buff, MAX_READ_LEN, "cmd=%d, file=%s", CMD_GET, path);

    if(write(sfd, buff, MAX_READ_LEN) == -1){
        HANDLE_ERROR("write");
    }
    memset(buff, '\0', MAX_READ_LEN);

    if(read(sfd, buff, MAX_READ_LEN) == -1){
        printf("%s\n", buff);
        HANDLE_ERROR("read");
    }
    if(strncmp(buff, CTS, strlen(CTS)) != 0){
        HANDLE_GET_FAILSAFE(sfd);
        return;
    }
    memset(buff, '\0', MAX_READ_LEN);
    
    int ffd = open((file_rename == NULL) ? path : file_rename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if(ffd == -1){
        HANDLE_ERROR("open");
    }

    stf_send(sfd, ffd);

    close(ffd);
}

void client_put(int sfd, char * path, char * file_rename){
    int ffd = open(path, O_RDONLY);
    if(ffd == -1){
        perror("open");
        return;
    }
    char buff[MAX_READ_LEN];
    memset(buff, '\0', MAX_READ_LEN);

    snprintf(buff, MAX_READ_LEN, "cmd=%d, file=%s", CMD_PUT, (file_rename == NULL) ? path : file_rename);

    if(write(sfd, buff, MAX_READ_LEN) == -1){
        HANDLE_ERROR("write");
    }
    memset(buff, '\0', MAX_READ_LEN);

    if(read(sfd, buff, MAX_READ_LEN) == -1){
        printf("%s\n", buff);
        HANDLE_ERROR("read");
    }
    if(strncmp(buff, CTS, strlen(CTS)) != 0){
        HANDLE_PUT_FAILSAFE(sfd);
        return;
    }

    fts_send(sfd, ffd);

    close(ffd);
}

void client_exit(int sfd){
    char buff[MAX_READ_LEN];
    memset(buff, '\0', MAX_READ_LEN);

    snprintf(buff, MAX_READ_LEN, "cmd=%d, file=N/A", CMD_EXIT);

    if(write(sfd, buff, MAX_READ_LEN) == -1){
        HANDLE_ERROR("write");
    }
}

// Server
void server_list(int sfd, char *path){
    DIR *dir;
    struct dirent *entry;
    char buffer[MAX_READ_LEN];
    memset(buffer, '\0', MAX_READ_LEN);

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        ALERT_UNEXPECTED_EVENT("Can't open serverside storage directory", sfd)
    }
    else if (write(sfd, CTS, sizeof(CTS)) == -1){
        HANDLE_ERROR("write");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){

            snprintf(buffer, sizeof(buffer), "%s\n", entry->d_name);

            if (write(sfd, buffer, strlen(buffer)) == -1) {
                closedir(dir);
                HANDLE_ERROR("write");
            }
        }
    }

    if(write(sfd, "\0", 1) == -1){
        HANDLE_ERROR("write");
    }

    closedir(dir);
}

void server_get(int sfd, char * storage_path, char *path){
    char buffer[MAX_READ_LEN];
    memset(buffer, '\0', MAX_READ_LEN);

    strcat(buffer, storage_path);
    strcat(buffer, path);

    int ffd = open(buffer, O_RDONLY);
    if(ffd == -1){
        perror("open");
        ALERT_UNEXPECTED_EVENT("server_get: Can't open requested file", sfd);
        return;
    }
    else if (write(sfd, CTS, sizeof(CTS)) == -1){
        HANDLE_ERROR("write");
    }

    fts_send(sfd, ffd);

    close(ffd);
}

void server_put(int sfd, char * storage_path, char *path){
    char buffer[MAX_READ_LEN];
    memset(buffer, '\0', MAX_READ_LEN);

    strcat(buffer, storage_path);
    strcat(buffer, path);

    int ffd = open(buffer, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(ffd == -1){
        perror("open");
        ALERT_UNEXPECTED_EVENT("server_put: File already exist or open failed", sfd);
        return;
    }
    else if (write(sfd, CTS, sizeof(CTS)) == -1){
        HANDLE_ERROR("write");
    }

    stf_send(sfd, ffd);

    close(ffd);
}

void server_exit(int sfd, int num){
    printf("Closing client %d connection\n", num);
    if(close(sfd) == -1){
        HANDLE_ERROR("close");
    }
    exit(EXIT_SUCCESS);
}
