#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
    struct passwd *pwd;
    uid_t u;
    pwd = getpwnam(argv[1]);
    if (pwd == NULL)
        return -1;
    
    DIR * dir;
    struct dirent * ptr;
    dir = opendir("/proc");
    while((ptr = readdir(dir)) != NULL){
        if(atoi(ptr -> d_name)){
            char path[100] = "/proc/";
            char *path1 = "/status";    
            strcat(path,ptr -> d_name);
            strcat(path,path1);

            FILE *f = fopen(path,"r");
            if(f == NULL) return 0;
            char *buf = NULL;
            size_t line = 100;
            char p_name[100] = "";
            char pid[50] = "";
            while(getline(&buf,&line,f) != -1){
                char *key = strtok(buf," \t\n");
                if(!strcmp(key,"Name:")){
                    strcpy(p_name,strtok(NULL," \t\n"));
                }
                if(!strcmp(key,"Pid:")){
                    strcpy(pid,strtok(NULL," \t\n"));
                }
                if(!strcmp(key,"Uid:")){
                    char *uid = strtok(NULL," \t\n");
                    if(atoi(uid) == pwd -> pw_uid){
                        printf("Name: %s  Pid: %s\n",p_name,pid);
                    }else {
                        fclose(f);
                        break;
                    }
                }
            }
        }
    }
    closedir(dir);
}