/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;
	socklen_t len;
	if (first_time) {  /* Set up server address & create local UDP socket */
		first_time = 0;

		/* Set up destination address.*/
		if((host = gethostbyname(resource)) == NULL){
			perror("gethostbyname error");
			exit(1);
		}
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr("127.0.0.1");
		server.sin_port = htons(PORT);
		len = sizeof(server);
		/* Allocate a socket.*/
		sockfd = socket(AF_INET,SOCK_DGRAM,0);
		if(sockfd < 0){
			perror("socket error");
			exit(1);
		}
	}

	/* Send a datagram & await reply*/
	sendto(sockfd,sought->word,strlen(sought->word),0,(struct sockaddr*) &server,sizeof(server));
	recvfrom(sockfd,sought->text,sizeof(sought->text),0,(struct sockaddr*) &server,&len);
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
