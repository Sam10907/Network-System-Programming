#include <sys/types.h> 
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct process_node{
    int pid;
    int ppid;
    int space;
    char name[100];
    struct process_node *child;
    struct process_node *father;
    struct process_node *sibling;
};
void traversal(struct process_node*);
static int count = 1;
int main(void){
    DIR * dir;
    struct dirent * ptr;
    dir = opendir("/proc");
    struct process_node *pn =  malloc(sizeof(struct process_node));
    int i = 0;
    while((ptr = readdir(dir)) != NULL){
        int pid = atoi(ptr -> d_name);
        if(pid){
            pn[i].pid = pid;
            pn[i].child = NULL;
            pn[i].father = NULL;
            pn[i].sibling = NULL;
            pn[i].space = 1;
            char path[100] = "/proc/";
            strcat(path,ptr -> d_name);
            strcat(path,"/status");
            FILE *f = fopen(path,"r");
            if(f == NULL) return 0;
            char *buf = NULL;
            size_t line = 100;
            while(getline(&buf,&line,f) != -1){
                char *key = strtok(buf," \t\n");
                if(!strcmp(key,"Name:")){
                    strcpy(pn[i].name,strtok(NULL," \t\n"));
                    continue;
                }
                if(!strcmp(key,"PPid:")){
                    pn[i].ppid = atoi(strtok(NULL," \t\n"));
                    break;
                }
            }
            pn = realloc(pn,(++i+1)*sizeof(struct process_node));
            fclose(f);
        }
    }
    int j = i - 1;
    int k;
    for(;j >= 0;j--){
        for(k = j - 1 ; k >= 0 ; k--){
            if(pn[j].ppid == pn[k].pid){
                pn[j].father = &pn[k];
                if(!pn[k].child) pn[k].child = &pn[j];
                else{
                    struct process_node *ptr = pn[k].child;
                    while(ptr -> sibling) ptr = ptr -> sibling;
                    ptr -> sibling = &pn[j];
                }
                break;
            }
        }
    }
    pn[0].sibling = &pn[1];
    printf("init\n");
    traversal(pn);
    free(pn);
}
void traversal(struct process_node *p){
    int num = p -> space;
    while(num--){ 
        printf("--");
    }
    printf("%s\n",p -> name);
    if(p -> child){
        count++;
        p -> child -> space = count;
        traversal(p -> child);
    }
    if(p -> sibling){
        if(p -> sibling -> pid == 2) count = 1;
        p -> sibling -> space = count;
        traversal(p -> sibling);
    }
    else{
        return;
    }
}
