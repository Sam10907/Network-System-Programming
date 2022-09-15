#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(int args,char *argv[]){
    int fd = open(argv[1],O_RDONLY | O_CREAT | O_APPEND,S_IRWXU);
    if(fd < 0){
        printf("open error\n");
        exit(1);
    }
    int buf_size = lseek(fd,0,SEEK_END);
    if(buf_size < 0){
        printf("lseek error\n");
        exit(1);
    }
    char *buf = malloc(buf_size);
    if(close(fd) < 0){
        printf("close error\n");
        exit(1);
    }
    //重新開啟檔案
    fd = open(argv[1],O_RDONLY | O_CREAT | O_APPEND,S_IRWXU);
    if(fd < 0){
        printf("open error\n");
        exit(1);
    }
    
    if(read(fd,buf,buf_size) < 0){
        printf("read error\n");
        exit(1);
    }
    if(close(fd) < 0){
        printf("close error\n");
        exit(1);
    }
    fd = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,S_IRWXU);
    if(fd < 0){
        printf("open error\n");
        exit(1);
    }
    if(write(fd,buf,buf_size) < 0){
        printf("write error\n");
        exit(1);
    }
    if(close(fd) < 0){
        printf("close error\n");
        exit(1);
    }
}