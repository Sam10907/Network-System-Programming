#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXSIZE 1024
int main(int args,char *argv[]){
    if(args < 2){
        printf("Usage:mycat filename\n");
        exit(1);
    }
    int fd = open(argv[1],O_RDONLY,S_IRUSR);
    if(fd < 0){
        perror("open error");
        exit(1);
    }
    char buf[MAXSIZE] = "";
    int n;
    while ((n = read(fd,buf,MAXSIZE)) > 0){
        if(write(1,buf,n) != n){
            perror("write error");
        }
    }
    if(n == -1) perror("read error");
    close(fd);
}