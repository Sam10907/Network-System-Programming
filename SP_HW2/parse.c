/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  	int count = 0;
  	char * token;
  	static char **newArgv;

  	/* Nothing entered. */
  	if (line == NULL) {
    	return NULL;
  	}

  	/* Init strtok with commandline, then get first token.
        Return NULL if no tokens in line. 
    */
   	token = strtok(line,delim);
	if(token == NULL) return NULL;


  	/* Create array with room for first token.*/
	int i = 0;
	newArgv = (char**) malloc(sizeof(char*));
	newArgv[i] =  (char*) malloc(strlen(token));
	strcpy(newArgv[i],token);
  	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 */
	printf("[%d] : %s\n",i,newArgv[i]);
	while(token = strtok(NULL,delim)){
		i++;
		newArgv = realloc(newArgv,(i+1)*sizeof(char*));
		newArgv[i] =  (char*) malloc(strlen(token));
		strcpy(newArgv[i],token);
		printf("[%d] : %s\n",i,newArgv[i]);
	}

  	/* Null terminate the array and return it.*/
	newArgv = realloc(newArgv,(i+2)*sizeof(char*));
	newArgv[i+1] = NULL;
  	return newArgv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv) {

	int i = 0;

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 */
	while(oldArgv[i]){
		free(oldArgv[i]);
		i++;
	}
	free(oldArgv);
}
