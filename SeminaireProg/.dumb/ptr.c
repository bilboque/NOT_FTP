#include <stdio.h>
#include <stdlib.h>

int main(){
    int * ptr;
    int * ptr2;
    ptr = malloc(sizeof(int));
    ptr2 = (int *) malloc(sizeof(int));

    printf("no cast: %p , cast: %p \n",ptr,ptr2);
    exit(EXIT_SUCCESS);
}
