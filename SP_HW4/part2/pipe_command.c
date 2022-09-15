/* 
 * pipe_command.c  :  deal with pipes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

#define STD_OUTPUT 1
#define STD_INPUT  0

void pipe_and_exec(char **myArgv) {
  	int pipe_argv_index = pipe_present(myArgv);
	int stat;
	pid_t pid;
	char **left_argv;
	char **right_argv;
	int i = 0;
	int pfd[2];
	if(pipe(pfd) == -1){
		perror("pipe error");
		exit(1);
	}
  	switch (pipe_argv_index) {

    	case -1:	/* Pipe at beginning or at end of argv;  See pipe_present(). */
      		fputs ("Missing command next to pipe in commandline.\n", stderr);
      		errno = EINVAL;	/* Note this is NOT shell exit. */
      		break;

    	case 0:	/* No pipe found in argv array or at end of argv array.
			See pipe_present().  Exec with whole given argv array. */
			
      		return;

    	default:	/* Pipe in the middle of argv array.  See pipe_present(). */

      		/* Split arg vector into two where the pipe symbol was found.
       		 * Terminate first half of vector.
			 */
			left_argv = (char**) malloc((pipe_argv_index + 1) * sizeof(char*)); //分配 "|" 左半邊的指令空間
			for(; i < pipe_argv_index ; i++){
				left_argv[i] = (char*) malloc(strlen(myArgv[i]));
				strcpy(left_argv[i],myArgv[i]);
			}
			left_argv[pipe_argv_index] = NULL;

			i = pipe_argv_index + 1;
			while(myArgv[i] != NULL && strcmp(myArgv[i],"|")){ 
				i++;
			}
			int end = i, j = 0;
			right_argv = (char**) malloc((end-pipe_argv_index) * sizeof(char*)); //分配 "|" 右半邊的指令空間
			for(i = pipe_argv_index + 1 ; i < end ; i++, j++){
				right_argv[j] = (char*) malloc(strlen(myArgv[i]));
				strcpy(right_argv[j],myArgv[i]);
			}
			right_argv[j+1] = NULL;

      		/* Create a new process for the right side of the pipe.
       		 * (The left side is the running "parent".)
       		 *
			 * Fill in code to replace the underline. */
      		switch((pid = fork())) {

        		case -1 :
	  				break;

        		/* Talking parent.  Remember this is a child forked from shell. */
        		default : //父
					close(pfd[1]);
					if(dup2(pfd[0],0) == -1) perror("2dup2 error");
					if(waitpid(pid,&stat,0) == -1){
						perror("waitpid error");
					}
					close(pfd[0]);
					pipe_and_exec(&myArgv[pipe_argv_index+1]);
					execvp(right_argv[0],right_argv);
        		/* Listening child. */
        		case 0 : //子
					close(pfd[0]);
					if(dup2(pfd[1],1) == -1){
						perror("1dup2 error");
					}
					close(pfd[1]);
					execvp(left_argv[0],left_argv);
			}
	}
	perror("Couldn't fork or exec child process");
  	exit(errno);
}
