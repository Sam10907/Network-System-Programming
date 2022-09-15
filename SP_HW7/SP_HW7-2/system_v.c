#include "system_v.h"
static struct sigaction sa;
sighandler_t my_sigset(int sig,sighandler_t handler){
    if(handler == SIG_HOLD){
        sigaddset(&sa.sa_mask,sig);
    }
    else if(handler == SIG_IGN){
        my_sigignore(sig);
    }
    else{
        sa.sa_handler = handler;
        sigemptyset(&sa.sa_mask);
        if(handler == SIG_DFL){
            sa.sa_flags = SA_RESETHAND | SA_NODEFER;
        }else sa.sa_flags = SA_RESTART;
        if(sigaction(sig,&sa,0) < 0)
            return SIG_ERR;
        else 
            return SIG_HOLD;
    }
}
int my_sighold(int sig){
    sigaddset(&sa.sa_mask,sig);
    int s;
    if((s = sigprocmask(SIG_BLOCK,&sa.sa_mask,0)) < 0){
        return -1;
    }
    return s;
}
int my_sigrelse(int sig){
    sigdelset(&sa.sa_mask,sig);
    int s;
    if((s = sigprocmask(SIG_UNBLOCK,&sa.sa_mask,0)) < 0){
        return -1;
    }
    return s;
}
int my_sigignore(int sig){
    sa.sa_handler = SIG_IGN;
    return sigaction(sig,&sa,0);
}
int my_sigpause(int sig){
    /*sigdelset(&sa.sa_mask,sig);
    int s;
    if((s = sigprocmask(SIG_UNBLOCK,&sa.sa_mask,0)) < 0){
        return -1;
    }*/
    my_sigrelse(sig);
    return sigsuspend(&sa.sa_mask);
}