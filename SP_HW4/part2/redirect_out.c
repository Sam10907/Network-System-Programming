/*
 * redirect_out.c   :   check for >
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_out(char ** myArgv) {
	int i = 0;
  	int fd;

  	/* search forward for >
  	*/
  	for(; myArgv[i] ; i++){
		if(!strcmp(myArgv[i],">")){
			break;
		}
	}
  	if (myArgv[i]) {	/* found ">" in vector. */

    	/* 1) Open file.
    	 * 2) Redirect to use it for output.
    	 * 3) Cleanup / close unneeded file descriptors.
    	 * 4) Remove the ">" and the filename from myArgv.
		*/
		close(STD_OUTPUT);
		freopen(myArgv[i+1],"w+",stdout);
		myArgv[i] = NULL;
		myArgv[i+1] = NULL;
		return 0;
  	}else{
		  return -1;
	  }
}
