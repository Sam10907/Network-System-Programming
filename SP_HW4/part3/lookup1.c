/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;

	if (first_time) { 
		first_time = 0;
		/* open up the file
		*/
		in = fopen(resource,"r");
		if(in == NULL) return 0;
	}

	/* read from top of file, looking for match
	*/
	rewind(in);
	char word[32] = "";
	char text[480] = "";
	strcat(sought -> word,"\n");
	while(fread(word,sizeof(char),32,in) != 0) {
		if(strcmp(word,sought -> word)){
			fread(text,sizeof(char),480,in);
		}else{
			sought -> word[strlen(word)-1] = '\0';
			fread(sought -> text,sizeof(char),480,in);
			return FOUND;
		}
	}
	sought -> word[strlen(sought -> word)-1] = '\0';
	return NOTFOUND;
}
