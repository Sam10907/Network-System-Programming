#include "system_v.h"
#include <stdio.h>
#include <unistd.h>

int main(void){
    siginfo_t si;
    sigset_t st;
    my_sighold(SIGINT);
    /*if (sigprocmask(SIGINT, &st, NULL) == -1){
        perror("sigprocmask");
    }*/
    //sigemptyset(&st);
    sleep(10);
    my_sigpause(SIGINT);
}