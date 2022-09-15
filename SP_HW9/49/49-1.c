#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]){
    int fd = open(argv[1],O_RDONLY);
    if(fd < 0){
        perror("open error");
        exit(1);
    }
    struct stat stat_buf;
    caddr_t addr, addr1;
    if(stat(argv[1],&stat_buf) < 0){
        perror("stat error");
        exit(1);
    }
    if((addr =(caddr_t) mmap(NULL,stat_buf.st_size,PROT_READ,MAP_SHARED,fd,0)) == NULL){
        perror("mmap error");
        exit(1);
    }
    close(fd);
    int fd1 = open(argv[2],O_RDWR | O_CREAT | O_TRUNC,S_IRWXU);
    if(fd1 < 0){
        perror("open1 error");
        exit(1);
    }
    if(ftruncate(fd1,stat_buf.st_size) < 0){
        perror("ftruncate error");
        exit(1);
    }
    if((addr1 =(caddr_t) mmap(NULL,stat_buf.st_size,PROT_WRITE,MAP_SHARED,fd1,0)) == NULL){
        perror("mmap1 error");
        exit(1);
    }
    memcpy(addr1,addr,stat_buf.st_size);
    close(fd1);
}