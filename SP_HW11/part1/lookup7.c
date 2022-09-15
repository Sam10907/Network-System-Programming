/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_un server;
	static int first_time = 1;
	int n;

	if (first_time) {     /* connect to socket ; resource is socket name */
		first_time = 0;

		/* Set up destination address.*/
		sockfd = socket(AF_UNIX,SOCK_STREAM,0);
		if(sockfd < 0){
			perror("socket error");
			exit(1);
		}
		/* Allocate socket. */
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path,resource);

		/* Connect to the server.*/
		if(connect(sockfd,(struct sockaddr*) &server,sizeof(server)) < 0){
			perror("connect error");
			exit(1);
		}
	}

	/* write query on socket ; await reply*/
    if(send(sockfd,sought->word,strlen(sought->word),0) < 0){
		perror("send error");
		exit(1);
	}
	if(recv(sockfd,sought->text,sizeof(sought->text),0) < 0){
		perror("recv error");
		exit(1);
	}
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
