#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
FILE* my_popen(char*,char*);
int my_pclose(FILE*);
static pid_t fdtopid[1024];
int main(int argc,char *argv[]){
    char com[50] = "";
    char mode[2];
    printf("open mode: ");
    scanf("%s",mode);
    printf("command:\n");
    while(read(0,com,sizeof(com)) != EOF){
        com[strlen(com)] = '\0';
        FILE *f = my_popen(com,mode);
        if(f == NULL){
            printf("my_popen error\n");
            return -1;
        }
        if(mode[0] == 'r'){
            char buf1[200] = "";
            while(fread(buf1,sizeof(char),sizeof(buf1),f) > 0){
                printf("%s",buf1);
            }
        }
        if(mode[0] == 'w'){
            char buf2[200] = "";
            while(read(0,buf2,sizeof(buf2)) > 0){
                int fd = fileno(f);
                if(write(fd,buf2,strlen(buf2)) < 0){
                    perror("write error");
                    exit(1);
                }
                memset(buf2,0,sizeof(buf2));
            }
        }
        my_pclose(f);
        printf("\n");
        printf("command:\n");
        memset(com,0,sizeof(com));
    }
}
FILE* my_popen(char *command,char *mode){
    int pfd[2];
    if(pipe(pfd) < 0){
        perror("pipe error");
        exit(1);
    }
    pid_t pid = fork();
    int status;
    if(pid == -1){
        perror("fork error");
        exit(1);
    }else if(pid == 0){
        if(!strcmp(mode,"r")){
            close(pfd[0]);
            if(dup2(pfd[1],STDOUT_FILENO) < 0){
                perror("dup2 error");
                exit(1);
            }
            close(pfd[1]);
        }else if(!strcmp(mode,"w")){
            close(pfd[1]);
            if(dup2(pfd[0],STDIN_FILENO) < 0){
                perror("dup2 error");
                exit(1);
            }
            close(pfd[0]);
        }
        execlp("/bin/sh","/bin/sh","-c",command,NULL);
    }else{
        int keep_fd;
        if(!strcmp(mode,"r")){
            close(pfd[1]);
            keep_fd = pfd[0];
        }else if(!strcmp(mode,"w")){
            close(pfd[0]);
            keep_fd = pfd[1];
        }
        fdtopid[keep_fd] = pid;
        return fdopen(keep_fd,mode);
    }
}
int my_pclose(FILE* fd){
    int keep_fd;
    if((keep_fd = fileno(fd)) < 0){
        perror("fileno error");
        exit(1);
    }
    pid_t pid = fdtopid[keep_fd];
    if(pid == 0){
        return -1;
    }
    int status;
    if(waitpid(pid,&status,0) < 0) return -1;
}
