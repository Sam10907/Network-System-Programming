/*
 * lookup5 : local file ; setup a memory map of the file
 *           Use bsearch. We assume that the words are already
 *           in dictionary order in a file of fixed-size records
 *           of type Dictrec
 *           The name of the file is what is passed as resource
 */

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "dict.h"

/*
 * This obscure looking function will be useful with bsearch
 * It compares a word with the word part of a Dictrec
 */

int dict_cmp(const void *a,const void *b) {
    return strcmp(((Dictrec *)a)->word,((Dictrec *)b)->word);
}

int lookup(Dictrec * sought, const char * resource) {
	static Dictrec * table;
	static int numrec;
	Dictrec * found;
	static int first_time = 1;

	if (first_time) {  /* table ends up pointing at mmap of file */
		int fd;
		long filsiz;

		first_time = 0;

		/* Open the dictionary.*/
		fd = open(resource,O_RDONLY);
		if(fd < 0){
			perror("open error");
			exit(1);
		}
		/* Get record count for building the tree. */
		filsiz = lseek(fd,0L,SEEK_END);
		numrec = filsiz / sizeof(Dictrec);

		/* mmap the data.*/
		table =(Dictrec*) mmap(NULL,filsiz,PROT_READ,MAP_SHARED,fd,0);
		if(table == NULL){
			perror("mmap error");
			exit(1);
		}
		close(fd);
	}
    
	/* search table using bsearch*/
	found = (Dictrec*) bsearch(sought,table,numrec,sizeof(Dictrec),dict_cmp);
	if (found) {
		strcpy(sought->text,found->text);
		return FOUND;
	}

	return NOTFOUND;
}
