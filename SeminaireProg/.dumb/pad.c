#include <stdio.h>

struct abc {
    char a;
    int c;
    char b;
};

int main(){

    struct abc var;
    printf("%lu\n",sizeof(var));
}
