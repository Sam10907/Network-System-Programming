/*
 * convert.c : take a file in the form 
 *  word1
 *  multiline definition of word1
 *  stretching over several lines, 
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv) {
	FILE *in;
	FILE *out;        /* defaults */
	char line[BIGLINE];
	static Dictrec dr, blank;
	
	/* If args are supplied, argv[1] is for input, argv[2] for output */
	if (argc==3) {
		if ((in =fopen(argv[1],"r")) == NULL){DIE(argv[1]);}
		if ((out =fopen(argv[2],"w")) == NULL){DIE(argv[2]);}	
	}
	else{
		printf("Usage: convert [input file] [output file].\n");
		return -1;
	}

	/* Main reading loop : read word first, then definition into dr */

	/* Loop through the whole file. */
	while (!feof(in)) {
		/* Create and fill in a new blank record.
		 * First get a word and put it in the word field, then get the definition
		 * and put it in the text field at the right offset.  Pad the unused chars
		 * in both fields with nulls.
		 */

		/* Read word and put in record.  Truncate at the end of the "word" field.
		*/
		int k = 0;
		for(k = 0 ; k < TEXT ; k++) blank.text[k] = '\0';
		for(k = 0 ; k < WORD ; k++) blank.word[k] = '\0';
		char c;
		int i = 0;
		char *buffer = NULL;
		size_t line1 = 100;
		if(getline(&buffer,&line1,in) != -1){
			for(; i < strlen(buffer) + 1 ; i++){ 
				blank.word[i] = buffer[i];
				if(i == 0 && buffer[i] == '\n'){ 
					break;
				}
			}
		}
		/* Read definition, line by line, and put in record.
		 */
		char *buf = NULL;
		size_t line = 200;
		int index = 0;
		while(getline(&buf,&line,in) != -1){
			if(!strcmp(buf,"\n")) break;
			else{
				int j;
				for(j = 0 ; j < strlen(buf)+1 ; j++,index++){
					blank.text[index] = buf[j];
				}
			}
		}
		/* Write record out to file.
		*/
		size_t st,st1;
		if(!feof(in)){
			st = fwrite(blank.word,sizeof(char),WORD,out);
			st1 = fwrite(blank.text,sizeof(char),TEXT,out);
			if(st == 0 || st1 == 0){
				perror("fwrite error");
			}
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}
