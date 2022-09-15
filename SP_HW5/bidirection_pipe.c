#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(void){
    char buf[200] = "";
    int stdout_dup = dup(STDOUT_FILENO);
    int stdin_dup = dup(STDIN_FILENO);
    while(read(stdin_dup,buf,sizeof(buf)) != EOF){
        int p1[2];
        int p2[2];
        if(pipe(p1) < 0){
            perror("pipe error");
            exit(1);
        }
        if(pipe(p2) < 0){
            perror("pipe error");
            exit(1);
        }
        int status;
        pid_t pid = fork();
        if(pid == -1){
            perror("fork error");
            exit(1);
        }
        else if(pid == 0){
            close(p1[1]);
            dup2(p1[0],STDIN_FILENO);
            char child_buf[200] = "";
            if(read(p1[0],child_buf,sizeof(child_buf)) < 0){
                perror("child read error");
                exit(1);
            }
            close(p1[0]);
            int i = 0;
            for(; child_buf[i] != '\0' ; i++){
                child_buf[i] = toupper(child_buf[i]);
            }
            close(p2[0]);
            dup2(p2[1],STDOUT_FILENO);
            if(write(p2[1],child_buf,strlen(child_buf)) < 0){
                perror("child write error");
                exit(1);
            }
            close(p2[1]);
        }else{
            close(p1[0]);
            dup2(p1[1],STDOUT_FILENO);
            buf[strlen(buf)] = '\0';
            if(write(p1[1],buf,strlen(buf)+1) < 0){
                perror("write error1");
                exit(1);
            }
            close(p1[1]);
            close(p2[1]);
            dup2(p2[0],STDIN_FILENO);
            char parent_buf[200] = "";
            if(read(p2[0],parent_buf,sizeof(parent_buf)) < 0){
                perror("read error1");
                exit(1);
            }
            close(p2[0]);
            if(write(stdout_dup,parent_buf,strlen(parent_buf)) < 0){
                perror("write error2");
                exit(1);
            }
            waitpid(pid,&status,0);
        }
        memset(buf,0,sizeof(buf));
    }
}