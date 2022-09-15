#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
    char *dir = getcwd(NULL,0);
    printf("%s\n",dir);
    free(dir);
}