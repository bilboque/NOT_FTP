#ifndef __OPTIONS_H__
#define __OPTIONS_H__

typedef struct address_and_port{
    char * ip;
    int port;
} ap;

ap * init_ap_from_stdin();
ap * init_ap_from_argv(int argc, char * argv[]);
ap * GET_AP(int argc, char * argv[]);

#endif
