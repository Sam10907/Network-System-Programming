/*
 * is_background.c :  check for & at end
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"

int is_background(char ** myArgv) {

  	if (*myArgv == NULL)
    	return 0;

  	/* Look for "&" in myArgv, and process it.
  	 *
	 *	- Return TRUE if found.
	 *	- Return FALSE if not found.
	*/
	char **ptr = myArgv;
	int i = 0;
	while(*ptr){
		if(strcmp(*ptr,"&") == 0) return i;
		ptr++;
		i++;
	}
	return 0;
}