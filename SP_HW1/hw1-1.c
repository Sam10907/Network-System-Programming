#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(int args,char *argv[]){
    while(1){
        char buf[1024] = "";
        if(read(0,buf,1024) <= 0){
            printf("read error\n");
            exit(1);
        }
        if(write(1,buf,strlen(buf)) <= 0){
            printf("write error\n");
            exit(1);
        }
        if(argv[1] != NULL && args <=2){
            int fd = open(argv[1],O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
            if(fd < 0){
                printf("open error\n");
                exit(1);
            }
            if(write(fd,buf,strlen(buf)) <= 0){
                printf("write error\n");
                exit(1);
            }
            if(close(fd) < 0){
                printf("close error\n");
                exit(1);
            }
        }
        if(argv[1] != NULL && !(strcmp(argv[1],"-a"))){
            int fd = open(argv[2],O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
            if(fd < 0){
                printf("open error\n");
                exit(1);
            }
            if(write(fd,buf,strlen(buf)) < 0){
                printf("write error\n");
                exit(1);
            }
            if(close(fd) < 0){
                printf("close error\n");
                exit(1);
            }
        }
    }
}