#ifndef __CMD_H__
#define __CMD_H__

#define QUIT "quit"

typedef struct cmd{
    char * type;
    char * file;
}cmd;

cmd * init_cmd();
void get_cmd(cmd * cmd);

void help();
void check_cmd(cmd * cmd);

#endif
