/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server;
	struct sockaddr_un client;
    int sd,cd,n;
    Dictrec tryit;
	socklen_t len;
    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.*/
    sd = socket(AF_UNIX,SOCK_STREAM,0);
	if(sd < 0){
		perror("socket error");
		exit(1);
	}
    /* Initialize address.*/
	server.sun_family = AF_UNIX;
	strncpy(server.sun_path,argv[2],sizeof(server.sun_path)-1);
	unlink(argv[2]);
    /* Name and activate the socket.*/
	if(bind(sd,(struct sockaddr*) &server,sizeof(server)) < 0){
		perror("bind error");
		exit(1);
	}
	if(listen(sd,5) < 0){
		perror("listen error");
		exit(1);
	}
    /* main loop : accept connection; fork a child to have dialogue */
    for (;;) {
		/* Wait for a connection.*/
		len = sizeof(client);
		if((cd = accept(sd,(struct sockaddr*) &client,&len)) < 0){
			perror("accept error");
			exit(1);
		}
		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				while (recv(cd,tryit.word,sizeof(tryit.word),0) >= 0) {

					/* Lookup request. */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
						case FOUND: 
							send(cd,tryit.text,strlen(tryit.text),0);
							break;
						case NOTFOUND: 
							strcpy(tryit.text,"XXXX");
							send(cd,tryit.text,strlen(tryit.text),0);
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
					memset(tryit.word,0,sizeof(tryit.word));
				} /* end of client dialog */

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
