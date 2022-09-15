#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main(void){
    time_t t;
    time(&t);
    struct tm *info;
    char buf[100];
    info = localtime(&t);
    strftime(buf,100,"%b %d(%a), %Y %l:%M %p",info);
    printf("%s\n",buf);
}