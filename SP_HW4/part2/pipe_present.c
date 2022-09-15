/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
	int index = 0;
	int i;
	int not_find = 1;
  	/* Search through myCurrentArgv for a match on "|". */
	for(i = 0 ; myCurrentArgv[i] ; i++){
		if(!strcmp(myCurrentArgv[i],"|")){
			not_find = 0;
			index = i;
			break;
		}
	}
	i = 0;
	while(myCurrentArgv[i]) i++;
  	if (!strcmp(myCurrentArgv[0],"|") || !strcmp(myCurrentArgv[i-1],"|")) {
    	return -1;

  	} else if (not_find) {
    	return 0;

  	} else {
    	/* In the middle. */
    	return index;
  	}
}
