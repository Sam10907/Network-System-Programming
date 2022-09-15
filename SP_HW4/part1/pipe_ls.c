#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void){
    int p[2];
    int stat;
    if(pipe(p) < 0){
        perror("pipe create error");
    }
    pid_t pid = fork();
    char buf[1024] = "";
    int len;
    switch(pid){
        case 0:
            close(p[0]);
            close(STDOUT_FILENO);
            if(dup(p[1]) != STDOUT_FILENO){
                perror("dup error");
                exit(1);
            }
            close(p[1]);
            execlp("ls","ls","-l",NULL);
        case -1:
            perror("fork error");
            exit(1);
        default:
            close(p[1]);
            close(STDIN_FILENO);
            if(dup(p[0]) != STDIN_FILENO){
                perror("dup error");
                exit(1);
            }
            while((len = read(p[0],buf,1023)) > 0){
                buf[len] = '\0';
                printf("%s",buf);
            }
            close(p[0]);
            if(waitpid(0,&stat,0) == -1){
                perror("waitpid error");
                exit(1);
            }
    }
}