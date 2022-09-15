#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
int main(void){
    DIR * dir;
    struct dirent * ptr;
    dir = opendir("/proc");
    while((ptr = readdir(dir)) != NULL){
        if(atoi(ptr -> d_name)){
            printf("pid %d:\n",atoi(ptr -> d_name));
            char path[100] = "/proc/";
            strcat(path,ptr -> d_name);
            strcat(path,"/fd");
            DIR *dir1 = opendir(path);
            struct dirent *ptr1;
            strcat(path,"/");
            char path1[100] = "";
            strcpy(path1,path);
            while((ptr1 = readdir(dir1)) != NULL){
                char buf[200] = "";
                if(atoi(ptr1 -> d_name)){
                    strcat(path,ptr1 -> d_name);
                    if(readlink(path,buf,sizeof(buf)) == -1){ 
                        fprintf(stderr,"readlink error: %s\n",strerror(errno));
                        return 0;
                    }
                    printf(" %s\n",buf);
                }
                strcpy(path,path1);
            }
        }
    }
}