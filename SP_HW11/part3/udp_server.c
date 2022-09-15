/*
 * udp_server : listen on a UDP socket ;reply immediately
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
	static struct sockaddr_in server,client;
	int sockfd,siz;
	Dictrec dr, *tryit = &dr;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.*/
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0){
		perror("socket error");
		exit(1);
	}
	/* Initialize address.*/
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);
	/* Name and activate the socket.*/
	if(bind(sockfd,(struct sockaddr*) &server,sizeof(server)) < 0){
		perror("bind error");
		exit(1);
	}
	for (;;) { /* await client packet; respond immediately */

		siz = sizeof(client); /* siz must be non-zero */

		/* Wait for a request.*/
		while (recvfrom(sockfd,tryit->word,sizeof(tryit->word),0,(struct sockaddr*) &client,&siz) >= 0) {
			/* Lookup request and respond to user. */
			switch(lookup(tryit,argv[1]) ) {
				case FOUND: 
					/* Send response.*/
					sendto(sockfd,tryit->text,strlen(tryit->text),0,(struct sockaddr*) &client,sizeof(client));
					break;
				case NOTFOUND : 
					/* Send response.*/
					strcpy(tryit->text,"XXXX");
					sendto(sockfd,tryit->text,strlen(tryit->text),0,(struct sockaddr*) &client,sizeof(client));
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */
			memset(tryit->word,0,sizeof(tryit->word));
		} /* end while */
	} /* end forever loop */
} /* end main */
