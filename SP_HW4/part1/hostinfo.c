#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
    struct utsname uts;
    if(uname(&uts) == -1){
        perror("uname error\n");
        exit(1);
    }
    printf("hostname: %s\n",uts.nodename);
    system("hostid");
}