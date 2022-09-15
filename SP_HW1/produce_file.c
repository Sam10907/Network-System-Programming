#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
    int fd = open("nullbyte.txt",O_CREAT | O_RDWR | O_TRUNC,S_IRWXU);
    write(fd,"sa\0\0\0dscdcsc\0\0\0\0ssccddsc",24);
    close(fd);
}