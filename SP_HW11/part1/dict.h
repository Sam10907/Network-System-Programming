/*
 * header file for progressive dictionary exercise
 */

#include <stdio.h>
#include <stdlib.h>

#define WORD 32
#define TEXT 480

#define FOUND 		0
#define NOTFOUND 	1
#define UNAVAIL 	2

/* Used in fifo and message queue examples */
#define IDSIZE 96

/* Used in socket examples. */
#define PORT 5678

#define DIE(x) perror(x),exit(1)

/* standard lookup structure */
typedef struct {
  char word[WORD];
  char text[TEXT];
} Dictrec;

int lookup(Dictrec * ,const char *);

/* used in fifo and message queue examples */
typedef struct {
  char word[WORD];	/* the word sought */
  char id[IDSIZE];	/* use this to reply */
} Client;

/* used by client in message queue example */
typedef struct {
  long type;
  Client content;
} ClientMessage;

/* used by server in message queue example */
typedef struct {
  long type;
  char text[TEXT];
} ServerMessage;

/* Template for the layout of shared memory */
typedef struct {
  int numrec;
  Dictrec table[1];
} Memory;
