/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server, client;
	int sd,cd,n;
	Dictrec tryit;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.*/
	sd = socket(AF_INET,SOCK_STREAM,0);
	if(sd < 0){
		perror("socket error");
		exit(1);
	}
	/* Initialize address.*/
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	socklen_t len = sizeof(client);
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
					/* Lookup the word , handling the different cases appropriately */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
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
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
