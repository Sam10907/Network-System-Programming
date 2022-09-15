#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
typedef void (*sighandler_t)(int);
static int recv_seq;
void capture_signal(int sig,siginfo_t *siginfo,void *context)  //抓取由父行程傳來的資料順序
{
    recv_seq = siginfo -> si_int;
}
int seqcmp(char *str)  //比較字串裡的序號與父行程所傳來的參數是否相等
{
    int seq, i = 1;
    char *key = strtok(str," \t");
    while(key){
        if(i > 4) return -1;
        if(seq = atoi(key)){
            return seq;
        }
        key = strtok(NULL," \t");
        i++;
    }
}
int main(int argc,char *argv[]){
    int i;
    int data_num = atoi(argv[1]);
    int rate = atoi(argv[2]);
    int consumer = atoi(argv[3]);
    int buf = atoi(argv[4]);
    int buffer_size = buf * 80;
    char *message = (char*) malloc(buffer_size);
    char msg[40] = "This is message ";
    char * path = "/SHM_DEMO";
    int shmid = shm_open(path,O_RDWR | O_CREAT | O_TRUNC,0666);
    if(shmid < 0){
        perror("shm_open");
        exit(1);
    }
    if(ftruncate(shmid,buffer_size) < 0){
        perror("ftruncate error");
        exit(1);
    }
    char *addr = mmap(NULL,buffer_size,PROT_WRITE | PROT_READ,MAP_SHARED,shmid,0);
    if(addr == (void*) -1){
        perror("mmap");
        exit(1);
    }
    char *ptr = addr;
    int *p1 = (int*) (ptr+80*(buf - 1)+44); //共用變數 用於互斥鎖(compare and swap)
    *p1 = 0;
    pid_t pid[consumer];
    for(i = 0 ; i < consumer ; i++){
        if((pid[i] = fork())){
            
        }
        if(pid[i] == 0){
            struct sigaction sig;
            sigemptyset(&sig.sa_mask);
            sig.sa_sigaction = capture_signal;
            sig.sa_flags = SA_SIGINFO;
            sigaction(SIGUSR1, &sig, NULL);
            pause();
            int shmid = shm_open(path,O_RDWR,0666);
            if(shmid < 0){
                perror("shm_open");
                exit(1);
            }
            char *addr = mmap(NULL,buffer_size,PROT_READ | PROT_WRITE,MAP_SHARED,shmid,0);
            if(addr == (void*) -1){
                perror("mmap");
                exit(1);
            }
            char *ptr = addr;
            int *p = (int*) (ptr+80*(buf - 1)+40); //統計收到的訊息總量
            int *p1 = (int*) (ptr+80*(buf - 1)+44);
            int b = 0;
            while(b < data_num){
                char split[30] = "";
                while(!__sync_bool_compare_and_swap(p1,0,1)){} //critical section
                sprintf(split,"%s\n",ptr);
                printf("%s\n",split);
                *p1 = 0; //release cs
                if(recv_seq == seqcmp(split)){
                     __sync_fetch_and_add(p, 1);
                }else{

                }
                ptr += 80;
                if((b % buf) == (buf-1)) ptr = addr;
                b++;
                if(b >= data_num){
                    munmap(addr,buffer_size);
                    shm_unlink(path);
                    exit(0);
                }
                pause();
            }
        }
        else if(pid[i] < 0){
            perror("fork");
            exit(1);
        }
    }
    int n, i1;
    for(n = 0 ; n < data_num ; n++){
        char seq[20] = "";
        sprintf(seq,"%d",n);
        char msg1[40] = "";
        strcpy(msg1,msg);
        char *seq_msg = strcat(msg1,seq);
        while(!__sync_bool_compare_and_swap(p1,0,1)){} //critical section
        strcpy(ptr,seq_msg); //寫入訊息
        *p1 = 0; //release cs
        union sigval val;
        val.sival_int = n;
        for(i1 = 0 ; i1 < consumer ; i1++){
            if(sigqueue(pid[i1],SIGUSR1,val) < 0){ //發送帶參數的訊號(signal)
                perror("sigqueue");
                exit(1);
            }
        }
        usleep(rate*1000); //父行程掛起
        ptr += 80;
        if((n % buf) == (buf-1)){
            ptr = addr;
        }
    }
    ptr = addr;
    int stat;
    for(i1 = 0 ; i1 < consumer ; i1++){
        waitpid(pid[i1],&stat,0);
    }
    int *p = (int*) (addr+80*(buf - 1)+40);
    float loss = 1 - (*p/(data_num*consumer));
    printf("Total message: %d\n",(data_num * consumer));
    printf("Sum of received messages by all consumers: %d\n",*p);
    printf("Loss rate: %f\n",loss);
    munmap(addr,buffer_size);
    shm_unlink(path);
    free(message);
}