/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"

void run_command(char **myArgv) {
    pid_t pid;
    int stat;

    /* Create a new child process.
	 */
    pid = fork();
    char **ptr = myArgv;

    switch (pid) {

        /* Error. */
        case -1 :
            perror("fork");
            exit(errno);

        /* Parent. */
        default :
            /* Wait for child to terminate.
			 */
            if(!is_background(myArgv)){
                if(waitpid(pid,&stat,0) == -1){
                    perror("waitpid");
                    exit(1);
                }
            }else{
                if(waitpid(pid,&stat,WNOHANG) == -1){
                    perror("waitpid");
                    exit(1);
                }
            }
            /* Optional: display exit status.  (See wstat(5).)
			 */
            if(WIFSIGNALED(stat)){
                printf("%s terminated by signal %d.\n",myArgv[0],WTERMSIG(stat));
            }else if(WIFEXITED(stat)){
                printf("%s exited with status %d.\n",myArgv[0],WEXITSTATUS(stat));
            }else if(WIFSTOPPED(stat)){
                printf("%s stopped by signal %d.\n",myArgv[0],WSTOPSIG(stat));
            }else if(WIFCONTINUED(stat)){
                printf("%s continued\n",myArgv[0]);
            }
            return;

        /* Child. */
        case 0 :
            /* Run command in child process.
			 */
            /* Handle error return from exec */
            while(*ptr){
                if(!strcmp(*ptr,"&")) break; 
                ptr++; 
            }
            *ptr = NULL;
            if(execvp(myArgv[0],myArgv) == -1){
                perror("execvp");
                exit(1);
            }
			exit(errno);
    }
}
