#include "parser.h"
#include "jobs.h"
#include "builtin.h"

#include <bits/types/siginfo_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <readline/readline.h>
#include <readline/history.h>

#define HANDLE_ERROR(str) {perror(str);}
#define HANDLE_UNEXPECTED_EVENT() {exit(EXIT_FAILURE);}
#define BUFF_SIZE 256

int background_job = 0;

static void child_process_signal(int signum, siginfo_t * siginfo, void * unused){
    pid_t pid_sig = siginfo->si_pid;
    int status;
    char buff[BUFF_SIZE];
    memset(buff, '\0', BUFF_SIZE);
    switch (signum) { 
        case SIGCHLD:
            if (pid_sig == background_job){
                waitpid(pid_sig, &status, 0);
                sprintf(buff, "\nbg job exited with code %d\n", status);
                write(STDOUT_FILENO, buff, sizeof(buff));
                background_job = 0;
            }
            break;
        default:
            break;
    }
}

void print_ps() {
    char username[BUFF_SIZE];
    char cwd[1024];

    if (getlogin_r(username, BUFF_SIZE) != 0) {
        perror("getlogin_r");
        exit(EXIT_FAILURE);
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf("\033[1;96m┌%s@trash - \033[1;97m%s\n", username, cwd);
    printf("\033[1;96m└$\033[0m");
}

int main(){
    struct sigaction sa;
    sa.sa_sigaction = child_process_signal;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_restorer = NULL;

    sigaction(SIGCHLD, &sa, NULL);

    rl_bind_key('\t', rl_complete);
    // Enable history
    using_history();
    
    char buff[BUFF_SIZE];
    while(1){
        memset(buff, '\0', BUFF_SIZE);
        print_ps();
        char* input = readline(" ");
        strncpy(buff, input, BUFF_SIZE);

        if (!input)
            break;
        else{
            args * arguments = parse(buff, " ");
            pid_t bg_pid = background_job;
            if(arguments == NULL){
                HANDLE_UNEXPECTED_EVENT();
            }

            switch (check_builtin(arguments->argv[0])) {
                case BUILTIN_CD:
                    builtin_cd(arguments->argv[1]);
                    break;
                case BUILTIN_EXIT:
                    builtin_exit();
                    break;
                default:
                    bg_pid = exec_job(arguments->argc, arguments->argv, background_job);
                    break;
            }
            if(bg_pid > 1){
                background_job = bg_pid;
            }

            free_args(arguments);
            free(input);
        }
    }
}
