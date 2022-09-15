#include "system_v.h"
#include <stdio.h>
#include <stdlib.h>
#define BUFSZ 80
/* This will be our signal handler for SIGINT */
//網路上抓來的測試範例
void sig_interrupt(int signal); 

int counter = 0; 
int main( int argc, char **argv, char **envp )
{
  char *buff = calloc(BUFSZ, sizeof(char));

  my_sigset(SIGINT, sig_interrupt);

  while ( 1 )
  {
    printf("Type something: ");
    fgets(buff, BUFSZ, stdin);		/* get a line */
    if ( buff[0] )
    {
      printf("You typed %s", buff);
      buff[0] = '\0';
    }
  }
} /* main() */

/* this is the signal handler, which will exit() after 3 SIGINTs */
void sig_interrupt(int signal)
{
  printf("SIGINT caught!\n");
  counter++;
  if ( counter == 3 )
  {
    printf("OK 3 SIGINTs, I give up\n");
    exit(3);
  }
} /* sig_interrupt() */
